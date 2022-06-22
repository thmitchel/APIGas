import pandas as pd
from datetime import datetime, date
import os
import time
os.environ["TZ"] = "America/Recife"
time.tzset()
from flask import Flask, request, render_template

app = Flask(__name__)


@app.route('/gas', methods = ['POST'])
def gas():
    tolken = 'SenhadaAPI'
    if(tolken == request.form.get('tolken')):
        temperatura = request.form.get('temperatura')
        concentracao = request.form.get('concentracao')
        umidade = request.form.get('umidade')

        gravaDados(temperatura, concentracao, umidade)
        return 'Temperatura: '+ str(temperatura) +  '\nConcentração: '+ str(concentracao) + '\nUmidade: '+ str(umidade)
    else: return 'Tolken inválido'

def gravaDados(temperatura, concentracao, umidade):
    data_atual = str(date.today())
    hora_atual = str(datetime.time(datetime.now()))
    hora_atual = hora_atual[0:5]
    dados = str(temperatura) + ',' + str(concentracao) + ',' + str(umidade) + ',' + str(data_atual) + ',' + str(hora_atual) + '\n'
    arquivo = open('/home/grupoapigas/mysite/dadosSalvos/dadosSalvos.csv ' ,'a')
    arquivo.write(dados)
    arquivo.close()
    return 'ok'

    #data_atual = str(date.today())
    #hora_atual = str(datetime.time(datetime.now()))
    #hora_atual = hora_atual[0:5]
    # funcao para mostrar ultimos dados lidos
def ultimosDados( dadosSalvos ) :
    df=pd.read_csv( dadosSalvos )
    conc = df.loc[len(df)-1, 'concentracao']
    temp = df.loc[len(df)-1, 'temperatura']
    umid = df.loc[len(df)-1, 'umidade']
    data_leitura = df.loc[len(df)-1, 'data']
    hora_leitura = df.loc[len(df)-1, 'hora']
    return  str(temp), str(umid), str(conc), data_leitura, hora_leitura
# --------------------------------------------------------------------

# endpoint para visualizar ( HTML ) estado atual / ultima leitura de temperatura, umidade e concentração
# http://fac.pythonanywhere.com/gas
@app.route('/mostra', methods=['GET'])
def inicio():
    nomearquivo = ('/home/grupoapigas/mysite/dadosSalvos/dadosSalvos.csv ')
    c1, u1, t1, d1, h1 = ultimosDados(nomearquivo)
    return render_template( 'dash.html', s1="Sensor", c1=c1, u1=u1, t1=t1, d1=d1, h1=d1)

# --------------------------------------------------------------------

@app.route('/apigas', methods=['GET'])
def paginaInicial():
    return render_template('paginaInicial.html')

