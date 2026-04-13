# Como usar:

# 1. Criar chaves para Alice e Bob
python3 cfich_nikesig.py setup alice
python3 cfich_nikesig.py setup bob

# 2. Criar um ficheiro de teste
echo "mensagem secreta" > mensagem.txt

# 3. Alice cifra para Bob
python3 cfich_nikesig.py enc bob alice mensagem.txt

# 4. Bob decifra
python3 cfich_nikesig.py dec bob alice mensagem.txt.enc