# Pesatura_Arnie
È un progetto sostenuto dall'associazione [Sermig](http://www.sermig.org/) e in particolare dal suo gruppo Restituzione Tecnologica.

Vista la situazione di attuale difficoltà del settore dell'apicoltura, dovuto a malattie delle api e inquinamento, è nata l'idea di un progetto di monitoraggio a distanza delle arnie, a basso costo, che aiuti materialmente gli apicoltori.

Il prototipo è stato costruito utilizzando una scheda di tipo Arduino, mentre nella fase di testing la scheda utilizzata è una NodeMCU. A essa sono collegati sensori di temperatura e umidità (DHT11) e un ponte di Wheatstone di celle di carico (amplificate con un HX711).
L'analisi dati è effettuata con [Root](https://root.cern.ch/) e tramite il sito https://thingspeak.com/.
