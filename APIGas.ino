#include <ESP8266WiFi.h>        /* bilblioteca adequada para wifi esp8266 */
#include <ESP8266HTTPClient.h>  /* biblioteca para acesso à API */
#include "DHT.h"                /* bib sensor dht */

#define api_header_key  "ueyr123768565HGHgjhgHGHJghjghgHDFgdfdhgfklkjlkjuytty68"
#define DHTPIN 4      // pino D2 do nodemcu
#define DHTTYPE 22    // especificacao do modelo de sensor DHT

#define http_ads2napp3 "grupoapigas.pythonanywhere.com/gas;  // url da API

#define IDMODULO "ID01- GAS"  // definição da identificação do modulo de leitura e placa

#define REDEWIFI   "Redmi 10"   // wifi a ser utilizada
#define SENHA       "12346710"  // senha da rede wifi a ser utilizada

/*
 * Variáveis globais
 */
unsigned long intervalo = 60000;  // variavel para definir intervalo entre leituras - cada 1000 equivale a um segundo
unsigned long tempoAnterior = 0;  // variavel para guardar o momento da leitura / envio anterior
int num_medida = 0;


DHT dht(DHTPIN, DHTTYPE); /* configura o pino e o tipo de sensor utilizado */


/*
   Inicialização / SETUP
   neste caso, conexao a rede wifi disponivel
*/
void setup () {

  Serial.begin(115200);
  WiFi.begin( REDEWIFI , SENHA );
  dht.begin();

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.print("Connectando à rede WIFI..:");
    Serial.println(REDEWIFI);
  }
  Serial.print("Connectado à rede WIFI ");
  Serial.println(REDEWIFI);
}

void loop() {
  String endpoint;
  char medida[10];
  static float temperatura;
  unsigned long tempoatual = millis();
  float concentracao;

  if ( (tempoatual - tempoAnterior) > intervalo )
  {
    tempoAnterior = tempoatual;
   
    if (WiFi.status() == WL_CONNECTED) { // Verificando o status da conexão WiFi

      HTTPClient http;  // Declaração de objeto da classe HTTPClient

      num_medida++;
      //deteccao = dht.readTemperature();

      if ( isnan( temperatura ) )   /* funcao paraa testar se a leitura foi bem sucedida */
      {
        Serial.println( "Erro na medicao\n" );
        delay(100);
      }
      else
      {
        env_Tempidkeyheader( temperatura, http_ads2napp3, IDMODULO2, KEYAPI );        
      }
    }
  }
}


/*
 * Função para preparar e realização uma requisição HTTP com informações de temperatura
 *
 * Http da api
 * identificação do sensor
 * chave do modulo para testar segurança
 */
void env_Tempidkeyheader ( float temperatura, char *http_api , char *id_api, char*chave_modulo) {
  String endpoint;
  char medida[10];
  HTTPClient http;  // Declarar  um objeto da classe HTTPClient

  Serial.println( http_api );
  http.begin( http_api ); // Especificando o endereço da requisição HTTP
  http.addHeader("Content-Type","application/x-www-form-urlencoded");
  http.addHeader("Authorization-Token", api_header_key );


  sprintf( medida, "%03d", (int)temperatura );
  endpoint = "api_key=1A2b3C4E5f";
  endpoint += "&id=";
  endpoint += id_api;
  endpoint += "&chave=";
  endpoint += chave_modulo;
  endpoint+=  "&concentracao="
  endpoint+= vibracao;
  endpoint += "&temperatura=";
  endpoint += medida;

  int httpCode = http.POST(endpoint); // Enviando a requisição

  if (httpCode > 0) {                           // Verificando o retorno da requisição
    String retorno = http.getString();    // obtendo o retorno da requisição
    Serial.println( retorno );            // mostrando o retorno
  }
  else {
    Serial.print("Erro na requisição:");
    Serial.println( http_api );
  }
  http.end();   //Close connection
}
