import subprocess
import os

pasta_testes = "exemplos_testes"
analisador = "anasinPASCAL.py"
for nome in sorted(os.listdir(pasta_testes)):
    if nome.endswith(".pas"):
        caminho = os.path.join(pasta_testes, nome)

        print(f"\n=== A correr {nome} ===")

        with open(caminho, "r", encoding="utf-8") as f:
            codigo = f.read()

        subprocess.run(
            ["python3", analisador],
            input=codigo,
            text=True
        )
