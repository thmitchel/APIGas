#include<ESP8266WiFi.h> //Biblioteca para funcionamento do WiFi do ESP
#include<ESP8266WebServer.h> //Biblioteca para o ESP funcionar como servidor
#include <DHT.h> //Biblioteca para funcionamento do sensor de temperatura e umidade DHT11
#include <ESP8266HTTPClient.h>

#define api_header_key  "SenhadaAPIGAS"

#define http_app2  "http://grupoapigas.pythonanywhere.com/gas"  // url da API

#define KEYAPI "ADS2NKEY01"

WiFiClient client;
HTTPClient httpClient;

int sensorGas = A0;
int concentracaoLimite = 1000;
//int sensorTemperatura = 
float temperatura;
float umidade;


const char* ssid = "DESKTOP-CBURNKH 5501";  // Rede WiFi
const char* password = "72y/789M";  //Senha da Rede WiFi

ESP8266WebServer server(80); //server na porta 80

#define DHTPIN 4 //Pino digital D2 (GPIO5) conectado ao DHT11
#define DHTTYPE DHT22 //Tipo do sensor DHT11

DHT dht(DHTPIN, DHTTYPE); //Inicializando o objeto dht do tipo DHT passando como parâmetro o pino (DHTPIN) e o tipo do sensor (DHTTYPE)

//Minha função para receber dados
void env_Dados ( float concentracao, char *http_app, float temperatura, float umidade ) {
  String endpoint;
  char medidaTemp[10];
  char medidaConc[10];
  char medidaUmi[10];

  Serial.println( http_app );
  httpClient.begin( client, http_app ); // Especificando o endereço da requisição HTTP
  httpClient.addHeader("Content-Type","application/x-www-form-urlencoded");
  httpClient.addHeader("Authorization-Token", api_header_key );

 sprintf( medidaTemp, "%03d", (int)temperatura );
 sprintf ( medidaConc, "%03d", (int)concentracao );
 sprintf ( medidaUmi, "%03d", (int)umidade);
  //endpoint = "";
  endpoint = "tolken=SenhadaAPI";
  endpoint += "&temperatura=";
  endpoint += medidaTemp;
  endpoint += "&concentracao=";
  endpoint += medidaConc; 
  endpoint += "&umidade=";  
  endpoint += medidaUmi;
  Serial.print(endpoint);

  int httpCode = httpClient.POST(endpoint); // Enviando a requisição 

  if (httpCode > 0) {                                 // Verificando o retorno da requisição
    String retorno = httpClient.getString();      // obtendo o retorno da requisição
    Serial.println( retorno );                  // mostrando o retorno
  }
  else {
    Serial.print("Erro na requisição:");
    Serial.println( http_app );
  }
  httpClient.end();   //Close connection

}

void setup() {
  Serial.begin(115200); //Inicializa a comunicação serial
  delay(50); // ?Intervalo para aguardar a estabilização do sistema
  dht.begin(); //Inicializa o sensor DHT11

  Serial.println("Conectando a Rede: "); //Imprime na serial a mensagem
  Serial.println(ssid); //Imprime na serial o nome da Rede Wi-Fi

  WiFi.begin(ssid, password); //Inicialização da comunicação Wi-Fi

  //Verificação da conexão
  while (WiFi.status() != WL_CONNECTED) { //Enquanto estiver aguardando status da conexão
    delay(1000);
    Serial.print("."); //Imprime pontos
  }
  //Serial.println("");
  Serial.println("Conectado");
  Serial.println("IP: ");
  //Serial.println(WiFi.localIP()); //Função para exibir o IP da ESP
  pinMode(sensorGas, INPUT);
  //Serial.begin(9600);
}

void loop() {

 String endpoint;
 float concentracao = analogRead(sensorGas);
 temperatura = dht.readTemperature();  //Realiza a leitura da temperatura
 umidade = dht.readHumidity(); //Realiza a leitura da umidade

  Serial.print("Pin A0: ");
  Serial.println(concentracao);
  Serial.print("Temperatura: ");
  Serial.print(temperatura); //Imprime no monitor serial o valor da temperatura lida
  Serial.println(" ºC");
  Serial.print("Umidade: ");
  Serial.print(umidade); //Imprime no monitor serial o valor da umidade lida
  Serial.println(" %");

 if (concentracao > concentracaoLimite & temperatura > 28)
 {
  Serial.println("Risco de explosão!!!");
 }
 else
 {
  Serial.println("Concentração segura!");
 }
 delay(1000);

 env_Dados(concentracao, http_app2, temperatura, umidade);

 delay(50000);
}
