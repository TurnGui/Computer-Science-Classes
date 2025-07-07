package module;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Album implements Serializable {
    private String nome;
    private String interprete;
    private int ano;
    private List<Musica> musicas;

    public Album(String nome, String interprete,
            int ano){
        this.nome = nome;
        this.interprete = interprete;
        this.ano = ano;
        this.musicas = new ArrayList<>();
    }
    
    public Album(Album al){
        this.nome = al.getNome();
        this.interprete = al.getInterprete();
        this.ano = al.getAno();
        
        this.musicas = new ArrayList<>();
        for (Musica m : al.getMusicas()) {
            this.musicas.add(m.clone());
        }
    }

    public String getNome(){
        return this.nome;
    }
    public void setNome(String nome){
        this.nome = new String(nome);
    }
    public String getInterprete() {
        return this.interprete;
    }

    public void setInterprete(String interprete) {
        this.interprete = new String(interprete);
    }
    public int getAno() {
        return this.ano;
    }

    public void setAno(int ano) {
        this.ano = ano;
    }

    public int getDuracao() {
        int total = 0;
        for (Musica m : this.musicas) {
            total += m.getDuracao();
        }
        return total;
    }

    public List<Musica> getMusicas(){
        return new ArrayList<>(musicas);
    }

    @Override
    public String toString(){
        StringBuffer sb = new StringBuffer();

        sb.append("\nÁlbum: "); sb.append(this.nome);
        sb.append("\nIntérprete: "); sb.append(this.interprete);
        sb.append("\nAno: "); sb.append(this.ano);
        sb.append("\nDuração Total: "); sb.append(this.getDuracao()).append(" minutos");
        sb.append("\n --- Músicas --- : "); 
        for (Musica m : musicas) {
            sb.append("\n").append(m.toString()).append("\n");
        }

        return sb.toString();
    }

    @Override
    public Album clone(){
        return new Album(this);
    }
}
