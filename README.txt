Budau George, 322CC
Tema3 - Protocoale de comunicatie


	Server - Mod de functionare:
Serverul este implementat sub forma unui API de tip REST.
Acesta are o serie de URL-uri expuse care sunt accesate prin cereri HTTP de tip GET sau de tip POST.
Daca cererea a fost efectuata corect, serverul intoarce un obiect de tip JSON, cu  mai  multe  campuri, printre care
enuntul pentru etapa urmatoare.
IP-ul serverului este: 185.118.200.35, iar portul este: 8081.

	Client - Mod de functionare:
Clientul trebuie sa realizeze cereri de tip GET si de tip POST pe URL-urile specificate in corpul raspunsului de pe server,
respectand cerintele  impuse.
Toate cererile sunt executate in aceeasi rulare de program.
Informatiile din raspunsul serverului, care tin de formularea cererii HTTP, sunt extrase folosind biblioteca Parson de
parsare de JSON.

	Mod de rezolvare:
- Pentru prima etapa am efectuat o cerere de tip GET pe url-ul /task1/start
- Pentru etapa a doua am efectuat o cerere de tip POST, unde m-am logat cu credentialele din campul "data". Pentru a parsa
formatul JSON din raspuns am creat o functie 'get_json_values' care imi intoarce informatiile necesare din JSON pentru
rezolvarea task-urilor aferente. Astfel, in cerere am trimis url-ul, content-type-ul, cele 2 cookie-uri si credentialele
pentru logare.
- Pentru etapa a treia am raspuns corect la cele 2 ghicitori si, impreuna cu id-ul din queryParams, am compus parametrii ce
sunt atasati cererii. Am extras din raspuns token-ul si am trimis cererea de tip GET, trimitand si cele 2 cookie-uri.
- Pentru etapa a patra am trimis o cerere de tip GET pentru obtinerea cheii, trimitand in continuare autorizarea, precum si
cele doua cookie-uri.
- Pentru ultima etapa am trimis o cerere de tip GET pe informatiile date de JSON-ul din "data". Pentru aceasta a fost nevoie
sa obtin adresa ip data de dns-ul ce se afla in url, portul acesteia (care este 80, fiind portul specific pentru "http"),
parametrii din url si, bineinteles, cele doua cookie-uri. Pentru finalizarea etapei, am trimis o cerere de tip POST cu
raspunsul exact asa cum a fost primit, in format JSON.
