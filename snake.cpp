//LIBRERIE
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <deque>
#include <fstream>

using namespace std;


//VARIABILI GLOBALI
const int grandezza_celle = 30;
const int numero_celle = 25;
const int offset = 75;

#define GIALLO_TESTO "\033[33m"
#define BIANCO_TESTO "\033[37m"
#define ROSSO_TESTO "\033[31m"
#define BLU_TESTO "\033[34m"
#define VERDE_TESTO "\033[32m"
#define MAGENTA_TESTO "\033[35m"

Color verde = {173, 204, 96, 255};
Color verde_scuro = {41, 51, 24, 255};
Color rosso_game_over = {71, 0, 0, 255};
Color viola_new_record = {103, 18, 166, 255};
Color nero = {0, 0, 0, 255};
Color oro_new_record = {255, 201, 60, 255};

Image icona_finestra;

Vector2 direzione_serpente = {0, 0};

bool serpente_stampato = false;

double tempo = 0;

int punteggio = 0;
bool punteggio_stampato_game_history = false;

bool gioco_iniziato = false;
bool pausa = false;
bool pausa_precedente = false;
bool pausa_stampata = false;

int record = 0;
bool nuovo_record = false;
bool record_stampato_game_history = false;

bool new_record_viola_giallo = true;

bool suono_finito = false;




//FUNZIONI
//pausa aggiornamento posizione del serpente
bool intervalloAggiornamenti(double intervallo)
{
	double tempo_attuale = GetTime();

	if(tempo_attuale - tempo >= intervallo)
	{
		tempo = tempo_attuale;
		return true;
	}
	else
	{
		return false;
	}
}

//controllo se il cibo è generato sul corpo del serpente
bool ciboGeneratoNelSerpente(Vector2 pos_cib, deque<Vector2> corp_serp)
{
	for(unsigned int i = 0; i < corp_serp.size(); i++)
	{
		if (Vector2Equals(corp_serp[i], pos_cib))
		{
			return true;
		}
	}
	return false;
}

//caricamento del punteggio record
void caricaSalvataggi()
{
	string file_salvataggi("File/salvataggio.txt");

	int valore_letto = 0;

	ifstream file(file_salvataggi);

	if(!file.is_open()) 
	{
		cerr << "Could not open the file - '" << file_salvataggi << "'" << endl;
	}

	while(file >> valore_letto) 
	{
		record = valore_letto;
	}

	file.close();
}

//salva il record su un file
void salvataggioRecord()
{
	ofstream file_salvataggio("File/salvataggio.txt");

	if(punteggio > record)
	{
		file_salvataggio << punteggio << endl;
		file_salvataggio.close();
	}
}




//CLASSI
//classe cibo
class Cibo
{
public:
	Vector2 posizione_cibo;
	Texture2D texture_cibo;


	Cibo(deque<Vector2> corp_serp)
	{
		Image sprite_cibo = LoadImage("sprite/sprite_cibo.png");
		texture_cibo = LoadTextureFromImage(sprite_cibo);
		UnloadImage(sprite_cibo);
		posizione_cibo = generaPosizioneCasuale(corp_serp);
	}

	~Cibo()
	{
		UnloadTexture(texture_cibo);
	}

	void stampaCibo()
	{
		DrawTexture(texture_cibo, offset + posizione_cibo.x * grandezza_celle, offset + posizione_cibo.y * grandezza_celle, WHITE);
	}

	Vector2 generaCellaCasuale()
	{
		float x = GetRandomValue(0, numero_celle - 1);
		float y = GetRandomValue(0, numero_celle - 1);
		return Vector2{x, y};
	}

	Vector2 generaPosizioneCasuale(deque<Vector2> CP)
	{
		
		Vector2 posizione_cibo = generaCellaCasuale();

		while(ciboGeneratoNelSerpente(posizione_cibo, CP))
		{
			posizione_cibo = generaCellaCasuale();
		}

		return posizione_cibo;
	}
};


//serpente
class Snake
{
public:
	deque<Vector2> corpo_serpente = {Vector2 {6, 9}, Vector2 {5, 9}, Vector2 {4, 9}};
	Vector2 direzione = {1, 0};

	Texture2D texture_corpo_serpente;

	Texture2D texture_testa_serpente;

	Texture2D texture_testa_serpente_morto;

	Texture2D texture_coda;

	Texture2D texture_curva;
	
	Texture2D texture_bocca_aperta;

	Texture2D texture_testa_curvata_1;
	Texture2D texture_testa_curvata_2;

	Texture2D texture_testa_curvata_aperta_1;
	Texture2D texture_testa_curvata_aperta_2;

	Texture2D texture_testa_curvata_serpente_morto_1;
	Texture2D texture_testa_curvata_serpente_morto_2;


	bool allunga_serpente = false;

	Snake()
	{
		texture_corpo_serpente = LoadTexture("sprite/sprite_corpo_serpente.png");

		texture_testa_serpente = LoadTexture("sprite/testa_serpente.png");

		texture_testa_serpente_morto = LoadTexture("sprite/testa_serpente_morto.png");
	
		texture_coda = LoadTexture("sprite/coda.png");

		texture_curva = LoadTexture("sprite/curva.png");

		texture_bocca_aperta = LoadTexture("sprite/testa_serpente_bocca_aperta.png");

		texture_testa_curvata_1 = LoadTexture("sprite/testa_serpente_curvata_1.png");
		texture_testa_curvata_2 = LoadTexture("sprite/testa_serpente_curvata_2.png");

		texture_testa_curvata_aperta_1 = LoadTexture("sprite/testa_curvata_bocca_aperta_1.png");
		texture_testa_curvata_aperta_2 = LoadTexture("sprite/testa_curvata_bocca_aperta_2.png");

		texture_testa_curvata_serpente_morto_1 = LoadTexture("sprite/testa_curvata_morto_1.png");
		texture_testa_curvata_serpente_morto_2 = LoadTexture("sprite/testa_curvata_morto_2.png");
	}

	~Snake()
	{
		UnloadTexture(texture_corpo_serpente);

		UnloadTexture(texture_testa_serpente);

		UnloadTexture(texture_testa_serpente_morto);

		UnloadTexture(texture_coda);

		UnloadTexture(texture_curva);

		UnloadTexture(texture_bocca_aperta);

		UnloadTexture(texture_testa_curvata_1);
		UnloadTexture(texture_testa_curvata_2);

		UnloadTexture(texture_testa_curvata_1);
		UnloadTexture(texture_testa_curvata_2);

		UnloadTexture(texture_testa_curvata_serpente_morto_1);
		UnloadTexture(texture_testa_curvata_serpente_morto_2);
	}

	bool vicinanzaCibo(Vector2 pos_cib, unsigned int i)
	{
		if((corpo_serpente[i].x - pos_cib.x <= 2 && corpo_serpente[i].x - pos_cib.x >= -2) && (corpo_serpente[i].y - pos_cib.y <= 2 && corpo_serpente[i].y - pos_cib.y >= -2))
		{
			return true;
		}

		return false;
	}

	void stampaSerpente(Vector2 pos_cib)
	{
		for (unsigned int i = 0; i < corpo_serpente.size(); i++)
		{
			//testa serpente curvata
			if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, 1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x - 1 && !vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_1, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, -1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x - 1 && !vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_2, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y + 1 && !vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_1, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {-1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y + 1 && !vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_2, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, -1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x + 1 && !vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_1, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, 1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x + 1 && !vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_2, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {-1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y - 1 && !vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_1, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y - 1 && !vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_2, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle }, 0, 1.0f, WHITE);
			}

			//testa curvata con bocca aperta
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, 1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x - 1 && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_aperta_1, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, -1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x - 1 && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_aperta_2, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y + 1 && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_aperta_1, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, { -1, 0 }) && corpo_serpente[i + 1].y == corpo_serpente[i].y + 1 && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_aperta_2, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, -1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x + 1 && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_aperta_1, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, 1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x + 1 && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_aperta_2, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {-1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y - 1 && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_aperta_1, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y - 1 && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_testa_curvata_aperta_2, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}

			//serpente vicino a cibo
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {1, 0}) && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_bocca_aperta, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {-1, 0}) && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_bocca_aperta, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, -1}) && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_bocca_aperta, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
			else if(serpente_stampato == false && i == 0 && Vector2Equals(direzione, {0, 1}) && vicinanzaCibo(pos_cib, i))
			{
				DrawTextureEx(texture_bocca_aperta, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}

			//testa curvata con serpente morto
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {0, 1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x - 1)
			{
				DrawTextureEx(texture_testa_curvata_serpente_morto_1, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {0, -1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x - 1)
			{
				DrawTextureEx(texture_testa_curvata_serpente_morto_2, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y + 1)
			{
				DrawTextureEx(texture_testa_curvata_serpente_morto_1, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {-1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y + 1)
			{
				DrawTextureEx(texture_testa_curvata_serpente_morto_2, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {0, -1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x + 1)
			{
				DrawTextureEx(texture_testa_curvata_serpente_morto_1, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {0, 1}) && corpo_serpente[i + 1].x == corpo_serpente[i].x + 1)
			{
				DrawTextureEx(texture_testa_curvata_serpente_morto_2, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {-1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y - 1)
			{
				DrawTextureEx(texture_testa_curvata_serpente_morto_1, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {1, 0}) && corpo_serpente[i + 1].y == corpo_serpente[i].y - 1)
			{
				DrawTextureEx(texture_testa_curvata_serpente_morto_2, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}

			//serpente morto
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {1, 0}))
			{
				DrawTextureEx(texture_testa_serpente_morto, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {-1, 0}))
			{
				DrawTextureEx(texture_testa_serpente_morto, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {0, -1}))
			{
				DrawTextureEx(texture_testa_serpente_morto, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
			else if(serpente_stampato && i == 0 && Vector2Equals(direzione, {0, 1}))
			{
				DrawTextureEx(texture_testa_serpente_morto, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}

			//serpente normale
			else if(i == 0 && Vector2Equals(direzione, {1, 0}))
			{
				DrawTextureEx(texture_testa_serpente, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}
			else if(i == 0 && Vector2Equals(direzione, {-1, 0}))
			{
				DrawTextureEx(texture_testa_serpente, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(i == 0 && Vector2Equals(direzione, {0, -1}))
			{
				DrawTextureEx(texture_testa_serpente, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle }, 270, 1.0f, WHITE);
			}
			else if(i == 0 && Vector2Equals(direzione, {0, 1}))
			{
				DrawTextureEx(texture_testa_serpente, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}

			//curve serpente
			else if(i != 0 && i != corpo_serpente.size() - 1 && ((corpo_serpente[i].x == corpo_serpente[i - 1].x + 1 && corpo_serpente[i].y == corpo_serpente[i + 1].y - 1) || (corpo_serpente[i].x == corpo_serpente[i + 1].x + 1 && corpo_serpente[i].y == corpo_serpente[i - 1].y - 1)))
			{
				DrawTextureEx(texture_curva, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}
			else if(i != 0 && i != corpo_serpente.size() - 1 && ((corpo_serpente[i].x == corpo_serpente[i - 1].x + 1 && corpo_serpente[i].y == corpo_serpente[i + 1].y + 1) || (corpo_serpente[i].x == corpo_serpente[i + 1].x + 1 && corpo_serpente[i].y == corpo_serpente[i - 1].y + 1)))
			{
				DrawTextureEx(texture_curva, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if(i != 0 && i != corpo_serpente.size() - 1 && ((corpo_serpente[i].x == corpo_serpente[i - 1].x - 1 && corpo_serpente[i].y == corpo_serpente[i + 1].y - 1) || (corpo_serpente[i].x == corpo_serpente[i + 1].x - 1 && corpo_serpente[i].y == corpo_serpente[i - 1].y - 1)))
			{
				DrawTextureEx(texture_curva, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}
			else if(i != 0 && i != corpo_serpente.size() - 1 && ((corpo_serpente[i].x == corpo_serpente[i - 1].x - 1 && corpo_serpente[i].y == corpo_serpente[i + 1].y + 1) || (corpo_serpente[i].x == corpo_serpente[i + 1].x - 1 && corpo_serpente[i].y == corpo_serpente[i - 1].y + 1)))
			{
				DrawTextureEx(texture_curva, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}

			//corpo serpente
			else if(corpo_serpente[i - 1].x == corpo_serpente[i].x && i != 0 && i != corpo_serpente.size() - 1 && i != 0)
			{
				DrawTextureEx(texture_corpo_serpente, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle }, 0, 1.0f, WHITE);
			}
			else if(corpo_serpente[i - 1].y == corpo_serpente[i].y && i != 0 && i != corpo_serpente.size() - 1 && i != 0)
			{
				DrawTextureEx(texture_corpo_serpente, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}

			//coda serpente
			else if(i == corpo_serpente.size() - 1 && corpo_serpente[i - 1].x - 1 == corpo_serpente[i].x)
			{
				DrawTextureEx(texture_coda, {offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 0, 1.0f, WHITE);
			}
			else if(i == corpo_serpente.size() - 1 && corpo_serpente[i - 1].x + 1 == corpo_serpente[i].x)
			{
				DrawTextureEx(texture_coda, {30 + offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 180, 1.0f, WHITE);
			}
			else if (i == corpo_serpente.size() - 1 && corpo_serpente[i - 1].y - 1 == corpo_serpente[i].y)
			{
				DrawTextureEx(texture_coda, {30 + offset + corpo_serpente[i].x * grandezza_celle, offset + corpo_serpente[i].y * grandezza_celle}, 90, 1.0f, WHITE);
			}
			else if(i == corpo_serpente.size() - 1 && corpo_serpente[i - 1].y + 1 == corpo_serpente[i].y)
			{
				DrawTextureEx(texture_coda, {offset + corpo_serpente[i].x * grandezza_celle, 30 + offset + corpo_serpente[i].y * grandezza_celle}, 270, 1.0f, WHITE);
			}
		}
	}

	void aggiornaPosizione()
	{
		corpo_serpente.push_front(Vector2Add(corpo_serpente[0], direzione));

		if(allunga_serpente)
		{
			allunga_serpente = !allunga_serpente;
		}
		else
		{
			corpo_serpente.pop_back();
		}
	}

	void reset()
	{
		corpo_serpente = {Vector2 {6, 9}, Vector2 {5, 9}, Vector2 {4, 9}};
		direzione = {1, 0};
		gioco_iniziato = false;
		punteggio = 0;
		pausa = false;
	}
};


//classe gioco
class Game 
{
public:
	Snake snake = Snake();
	Cibo cibo = Cibo(snake.corpo_serpente);
	
	Sound scontro_muro;
	Sound curva;
	Sound cibo_mangiato;
	Sound start;
	Sound game_over;
	Sound vittoria;


	Game()
	{
		InitAudioDevice();

		scontro_muro = LoadSound("suoni/scontro_muro.mp3");
		curva = LoadSound("suoni/curva.mp3");
		cibo_mangiato = LoadSound("suoni/cibo_mangiato.mp3");
		start = LoadSound("suoni/musica_start.mp3");
		game_over = LoadSound("suoni/game_over.mp3");
		vittoria = LoadSound("suoni/vittoria.mp3");
	}

	~Game()
	{
		UnloadSound(scontro_muro);
		UnloadSound(curva);
		UnloadSound(cibo_mangiato);
		UnloadSound(start);
		UnloadSound(game_over);
		UnloadSound(vittoria);

		CloseAudioDevice();
	}
	
	void stampaCiboESerpente()
	{
		cibo.stampaCibo();
		snake.stampaSerpente(cibo.posizione_cibo);
	}

	void gameOver()
	{
		bool pausa_game_over = false;

		cout<< ROSSO_TESTO << "\tGAME OVER!\n";

		PlaySound(game_over);

		while(pausa_game_over == false)
		{
			BeginDrawing();
			ClearBackground(verde);

			if(!serpente_stampato)
			{
				serpente_stampato = !serpente_stampato;
				snake.corpo_serpente.pop_front();
			}

			snake.stampaSerpente(cibo.posizione_cibo);

			if(IsKeyPressed(KEY_R) && !IsSoundPlaying(game_over))
			{
				pausa_game_over = true;
			}

			DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)grandezza_celle * numero_celle + 10, (float)grandezza_celle * numero_celle + 10 }, 5, verde_scuro);
			
			DrawText("Game Over", 240, 460, 80, rosso_game_over);
			DrawText("Snake", offset - 5, 20, 50, verde_scuro);
			DrawText("Record:", 560, 50, 20, verde_scuro);
			DrawText(TextFormat("%i", record), 650, 35, 40, verde_scuro);
			DrawText("Score:", offset - 5, offset + grandezza_celle * numero_celle + 30, 20, verde_scuro);
			DrawText(TextFormat("%i", punteggio), offset + 70, offset + grandezza_celle * numero_celle + 15, 40, verde_scuro);
			
			if(punteggio > record && !record_stampato_game_history)
			{
				cout<< MAGENTA_TESTO << "\tNEW RECORD:\t" << punteggio << "\n\n";
				record_stampato_game_history = !record_stampato_game_history;
			}
			else if(punteggio > record)
			{
				if(new_record_viola_giallo)
				{
					DrawText("NEW RECORD:", 240, 550, 20, viola_new_record);
					DrawText(TextFormat("%i", punteggio), 610, 550, 20, viola_new_record);
					
					if(intervalloAggiornamenti(0.5))
					{
						new_record_viola_giallo = !new_record_viola_giallo;
					}
				}
				else if(new_record_viola_giallo == false)
				{
					DrawText("NEW RECORD:", 240, 550, 20, oro_new_record);
					DrawText(TextFormat("%i", punteggio), 610, 550, 20, oro_new_record);

					if(intervalloAggiornamenti(0.5))
					{
						new_record_viola_giallo = !new_record_viola_giallo;
					}
				}
			}
			else if(punteggio <= record && !punteggio_stampato_game_history)
			{
				cout<< BLU_TESTO << "\tPOINTS:\t" << punteggio << "\n\n";
				punteggio_stampato_game_history = !punteggio_stampato_game_history;
			}
			else if(punteggio <= record)
			{
				DrawText("Score:", 240, 550, 20, nero);
				DrawText(TextFormat("%i", punteggio), 610, 550, 20, nero);
			}

			EndDrawing();
		}

		if(record_stampato_game_history)
		{
			record_stampato_game_history = !record_stampato_game_history;
		}
		if (punteggio_stampato_game_history)
		{
			punteggio_stampato_game_history = !punteggio_stampato_game_history;
		}

		cout << GIALLO_TESTO << "\tNEW GAME\n";

		salvataggioRecord();
		snake.reset();
		cibo.posizione_cibo = cibo.generaPosizioneCasuale(snake.corpo_serpente);
		serpente_stampato = !serpente_stampato;
		suono_finito = !suono_finito;
	}

	void collisioneConbordi()
	{
		if(snake.corpo_serpente[0].x == numero_celle || snake.corpo_serpente[0].x == -1)
		{
			PlaySound(scontro_muro);
			gameOver();
		}

		if (snake.corpo_serpente[0].y == numero_celle || snake.corpo_serpente[0].y == -1)
		{
			PlaySound(scontro_muro);
			gameOver();
		}
	}

	void collisioneConCorpo()
	{
		deque<Vector2> corpo_senza_testa = snake.corpo_serpente;
		corpo_senza_testa.pop_front();

		for(unsigned int i = 0; i < corpo_senza_testa.size(); i++)
		{
			if (Vector2Equals(snake.corpo_serpente[0], corpo_senza_testa[i]))
			{
				PlaySound(scontro_muro);
				gameOver();
			}
		}
	}

	void collisioneConCibo()
	{
		if(Vector2Equals(snake.corpo_serpente[0], cibo.posizione_cibo))
		{
			PlaySound(cibo_mangiato);
			cibo.posizione_cibo = cibo.generaPosizioneCasuale(snake.corpo_serpente);
			snake.allunga_serpente = true;
			punteggio++;
			cout << GIALLO_TESTO << "\tFOOD EATEN\n";
		}
	}

	void win()
	{
		bool pausa_game_win = false;

		cout << GIALLO_TESTO << "\tYOU WIN!\n";

		PlaySound(vittoria);

		while (pausa_game_win == false)
		{
			BeginDrawing();
			ClearBackground(verde);

			snake.stampaSerpente(cibo.posizione_cibo);

			if (IsKeyPressed(KEY_R) && !IsSoundPlaying(vittoria))
			{
				pausa_game_win = true;
			}

			DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)grandezza_celle * numero_celle + 10, (float)grandezza_celle * numero_celle + 10 }, 5, verde_scuro);

			DrawText("YOU WIN!", 260, 460, 80, oro_new_record);
			DrawText("Snake", offset - 5, 20, 50, verde_scuro);
			DrawText("Record:", 560, 50, 20, verde_scuro);
			DrawText(TextFormat("%i", record), 650, 35, 40, verde_scuro);
			DrawText("Score:", offset - 5, offset + grandezza_celle * numero_celle + 30, 20, verde_scuro);
			DrawText(TextFormat("%i", punteggio), offset + 70, offset + grandezza_celle * numero_celle + 15, 40, verde_scuro);

			if (punteggio > record && !record_stampato_game_history)
			{
				cout << MAGENTA_TESTO << "\tNEW RECORD:\t" << punteggio << "\n\n";
				record_stampato_game_history = !record_stampato_game_history;
			}
			else if (punteggio > record)
			{
				if (new_record_viola_giallo)
				{
					DrawText("NEW RECORD:", 240, 550, 20, viola_new_record);
					DrawText(TextFormat("%i", punteggio), 610, 550, 20, viola_new_record);

					if (intervalloAggiornamenti(0.5))
					{
						new_record_viola_giallo = !new_record_viola_giallo;
					}
				}
				else if (new_record_viola_giallo == false)
				{
					DrawText("NEW RECORD:", 240, 550, 20, oro_new_record);
					DrawText(TextFormat("%i", punteggio), 610, 550, 20, oro_new_record);

					if (intervalloAggiornamenti(0.5))
					{
						new_record_viola_giallo = !new_record_viola_giallo;
					}
				}
			}
			else if (punteggio <= record && !punteggio_stampato_game_history)
			{
				cout << BLU_TESTO << "\tPOINTS:\t" << punteggio << "\n\n";
				punteggio_stampato_game_history = !punteggio_stampato_game_history;
			}
			else if (punteggio <= record)
			{
				DrawText("Score:", 240, 550, 20, nero);
				DrawText(TextFormat("%i", punteggio), 610, 550, 20, nero);
			}

			EndDrawing();
		}

		if (record_stampato_game_history)
		{
			record_stampato_game_history = !record_stampato_game_history;
		}
		if (punteggio_stampato_game_history)
		{
			punteggio_stampato_game_history = !punteggio_stampato_game_history;
		}

		cout << GIALLO_TESTO << "\tNEW GAME\n";

		salvataggioRecord();
		snake.reset();
		cibo.posizione_cibo = cibo.generaPosizioneCasuale(snake.corpo_serpente);
		suono_finito = !suono_finito;
	}

	void controlloPunteggio()
	{
		if(punteggio == 200)
		{
			win();
		}
	}

	void aggiornamenti()
	{
		snake.aggiornaPosizione();
		collisioneConCibo();
		collisioneConbordi();
		collisioneConCorpo();
		controlloPunteggio();
	}
};





//MAIN
int main()
{
	cout<< "Game starting...\n\n";

	InitWindow((2 * offset) + (grandezza_celle * numero_celle), (2 * offset) + (grandezza_celle * numero_celle), "Snake");
	icona_finestra = LoadImage("sprite/icona.png");
	SetWindowIcon(icona_finestra);
	UnloadImage(icona_finestra);

	SetTargetFPS(60);

	//oggetti
	Game game = Game();
	
	system("cls");

	//stampa comandi
	cout << GIALLO_TESTO;
	cout<< "\n";
	cout<< "KEY_UP:      MOVE UP\n";
	cout<< "KEY_DOWN:    MOVE DOWN\n";
	cout<< "KEY_LEFT:    MOVE LEFT\n";
	cout<< "KEY_RIGHT:   MOVE RIGHT\n";
	cout<< "PAUSE:       P\n";
	cout<< "RESET:       R\n";
	cout<< "EXIT:        ESC\n\n";
	cout<< "GAME HISTORY\n";


	cout<< GIALLO_TESTO << "\tNEW GAME\n";

	PlaySound(game.start);


	//game loop
	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(verde);

		caricaSalvataggi();

		if(intervalloAggiornamenti(0.17) && gioco_iniziato == true && pausa == false)
		{
			game.aggiornamenti();
		}

		if(pausa)
		{
			DrawText("Pause", offset + 15, offset - 25 + grandezza_celle * numero_celle, 15, nero);
			
			if(!pausa_stampata)
			{
				cout<< VERDE_TESTO << "\tGAME IN PAUSE\n";
				pausa_stampata = !pausa_stampata;
			}
		}

		//comandi
		//su
		if(IsKeyPressed(KEY_UP) && game.snake.direzione.y != 1 && pausa == false && !IsSoundPlaying(game.start))
		{
			game.snake.direzione = {0, -1};

			if(!Vector2Equals(game.snake.direzione, direzione_serpente))
			{
				PlaySound(game.curva);
			}
			
			direzione_serpente = game.snake.direzione;

			gioco_iniziato = true;
		}

		//giù
		if(IsKeyPressed(KEY_DOWN) && game.snake.direzione.y != -1 && pausa == false && !IsSoundPlaying(game.start))
		{
			game.snake.direzione = {0, 1};

			if(!Vector2Equals(game.snake.direzione, direzione_serpente))
			{
				PlaySound(game.curva);
			}

			direzione_serpente = game.snake.direzione;

			gioco_iniziato = true;
		}

		//sinistra
		if(IsKeyPressed(KEY_LEFT) && game.snake.direzione.x != 1 && pausa == false && !IsSoundPlaying(game.start))
		{
			game.snake.direzione = {-1, 0};

			if(!Vector2Equals(game.snake.direzione, direzione_serpente))
			{
				PlaySound(game.curva);
			}

			direzione_serpente = game.snake.direzione;
		}

		//destra
		if(IsKeyPressed(KEY_RIGHT) && game.snake.direzione.x != -1 && pausa == false && !IsSoundPlaying(game.start))
		{
			game.snake.direzione = {1, 0};

			if(!Vector2Equals(game.snake.direzione, direzione_serpente))
			{
				PlaySound(game.curva);
			}
			
			direzione_serpente = game.snake.direzione;

			gioco_iniziato = true;
		}
		if(IsKeyPressed(KEY_R) && pausa == false && !IsSoundPlaying(game.start))
		{
			PlaySound(game.scontro_muro);
			game.snake.reset();
			game.cibo.posizione_cibo = game.cibo.generaPosizioneCasuale(game.snake.corpo_serpente);
			cout<< BLU_TESTO << "\tRESET\n";
		}
		if (IsKeyPressed(KEY_P) && !IsSoundPlaying(game.start))
		{
			PlaySound(game.cibo_mangiato);
			pausa_precedente = pausa;
			pausa = !pausa;

			if(!pausa && pausa_precedente)
			{
				cout<< VERDE_TESTO << "\tGAME RUNNING\n";
				pausa_stampata = !pausa_stampata;
			}
		}


		game.stampaCiboESerpente();
		

		DrawRectangleLinesEx(Rectangle {(float)offset - 5, (float)offset - 5, (float)grandezza_celle * numero_celle + 10, (float)grandezza_celle * numero_celle + 10}, 5, verde_scuro);
		
		DrawText("Snake", offset - 5, 20, 50, verde_scuro);
		DrawText("Record:", 560, 50, 20, verde_scuro);
		DrawText(TextFormat("%i", record), 650, 35, 40, verde_scuro);
		DrawText("Score:", offset - 5, offset + grandezza_celle * numero_celle + 30, 20, verde_scuro);
		DrawText(TextFormat("%i", punteggio), offset + 70, offset + grandezza_celle * numero_celle + 15, 40, verde_scuro);

		EndDrawing();

		if(suono_finito)
		{
			PlaySound(game.start);
			suono_finito = !suono_finito;
		}
	}

	CloseWindow();

	cout<< ROSSO_TESTO << "\n<---- ! WINDOW CLOSED ! ---->\n\n";
	cout<< BIANCO_TESTO;

	return 0;
}