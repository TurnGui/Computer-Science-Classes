### Resolução das questões Semana 9

#### Q1 — Como provar que ALICE.key e ALICE.crt são par válido:
Compara o módulo N das duas chaves (com openssl rsa -modulus e openssl x509 -modulus). Se forem iguais, são o mesmo par RSA. Em Python compara priv_key.private_numbers().public_numbers.n com cert.public_key().public_numbers().n.

#### Q2 — Campos importantes a verificar:
Os campos mais críticos a verificar são:
Issuer (Emissor). Identifica quem emitiu o certificado. Deve ser uma CA de confiança. Sem verificar o emissor, qualquer um pode criar um certificado assinado falso. No código verificamos com cert.verify_directly_issued_by(ca_cert).
Validity (Período de Validade) — Contém as datas Not Before e Not After. A data atual deve estar dentro deste período. Certificados expirados ou ainda não válidos não devem ser aceites. Verificamos com cert_validtime(cert).
Common Name(CN) É o identificador do titular do certificado. O CN deve corresponder ao utilizador esperado. Este campo é crítico: sem verificar o CN, um certificado válido mas de outro utilizador (por exemplo, de Eva) seria aceite quando esperávamos um de Alice. Verificamos com cert_validsubject(cert, [(x509.NameOID.COMMON_NAME, "CSI ALICE")]).
Key Usage Define para que a chave pode ser usada (Digital Signature, Key Encipherment, etc.). Devemos verificar que o certificado permite Digital Signature para garantir que pode ser usado para assinar mensagens. No código: if not key_usage.digital_signature: raise Error().
Signature (Assinatura do Certificado) , A assinatura digital da CA sobre todo o certificado. Esta verificação é automática quando chamamos verify_directly_issued_by() e prova que o certificado foi realmente emitido por CA, pois é impossível falsificar sem a chave privada da CA.

#### Q3 — Erros provocados:
Na validação do certificado: apresentar um certificado com CN errado, self-signed (não emitido pela CA), ou expirado. Na validação da assinatura: usar a chave pública de BOB para verificar uma assinatura feita por ALICE, ou alterar a mensagem depois de assinar.
