package module.user;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import module.Album;
import module.Musica;
import module.PlanoSubscricao;
import module.Playlist;
import module.PlaylistAleatoria;
import module.PlaylistConstruida;
import module.PlaylistFavoritos;
import module.PlaylistPorGeneroDuracao;

public class Utilizador implements Serializable {
    private String nome;
    private String email;
    private String morada;
    private PlanoSubscricao plano;
    private double pontos;
    private int totalMusicasOuvidas; 
    private List<Playlist> playlists;
    private List<Album> biblioteca;
    private List<Musica> favoritos;

    public Utilizador(String nome, String email, String morada, PlanoSubscricao plano) {
        this.nome = nome;
        this.email = email;
        this.morada = morada;
        this.plano = plano;
        this.pontos = 0.0;
        this.totalMusicasOuvidas = 0; 
        this.playlists = new ArrayList<>();
        this.biblioteca = new ArrayList<>();
        this.favoritos = new ArrayList<>();

        if (plano == PlanoSubscricao.PREMIUM_TOP) {
            this.pontos += 100.0;
        }
    }

    public Utilizador(Utilizador ut) {
        this.nome = ut.getNome();
        this.email = ut.getEmail();
        this.morada = ut.getMorada();
        this.plano = ut.getPlano();
        this.pontos = ut.getPontos();
        this.totalMusicasOuvidas = ut.getTotalMusicasOuvidas(); 
        this.playlists = new ArrayList<>();
        this.biblioteca = new ArrayList<>();
        this.favoritos = new ArrayList<>();
    }

    public void reproduzirMusica(Musica m) {
        m.reproduzir();
        totalMusicasOuvidas++;

        switch (plano) {
            case FREE:
                this.pontos += 5;
                break;
            case PREMIUM_BASE:
                this.pontos += 10;
                break;
            case PREMIUM_TOP:
                this.pontos += this.pontos * 0.025;
                break;
        }
    }

    
    public int getTotalMusicasOuvidas() {
        return totalMusicasOuvidas;
    }

    public boolean isPremium() {
        return this.plano == PlanoSubscricao.PREMIUM_BASE || this.plano == PlanoSubscricao.PREMIUM_TOP;
    }

    public boolean isPremiumtop(){
        return this.plano == PlanoSubscricao.PREMIUM_TOP;
    }

    public void adicionarAlbum(Album a) {
        if (isPremium()) {
            biblioteca.add(a.clone());
        } else {
            System.out.println("Plano Free não permite adicionar álbuns à biblioteca.");
        }
    }

    public void adicionarPlaylist(Playlist p) {
        this.playlists.add(p.clone());
    }

    public List<Playlist> getPlaylists() {
        return new ArrayList<>(this.playlists);
    }

    public String getNome() {
        return this.nome;
    }

    public void setNome(String nome) {
        this.nome = new String(nome);
    }

    public String getEmail() {
        return this.email;
    }

    public void setEmail(String email) {
        this.email = new String(email);
    }

    public String getMorada() {
        return morada;
    }

    public void setMorada(String morada) {
        this.morada = new String(morada);
    }

    public PlanoSubscricao getPlano() {
        return plano;
    }

    public double getPontos() {
        return pontos;
    }

    public void adicionarPontos(double pontos) {
        this.pontos += pontos;
    }

    public void setPontos(int pontos) {
        this.pontos = pontos;
    }

    public List<Album> getBiblioteca() {
        return new ArrayList<>(biblioteca);
    }

    public void criarPlaylistConstruida(String nome, boolean aleatoria) {
        if (plano == PlanoSubscricao.FREE) {
            System.out.println("Utilizadores FREE não podem criar playlists construídas.");
            return;
        }

        PlaylistConstruida nova = new PlaylistConstruida(nome, aleatoria);
        playlists.add(nova);
    }

    public void criarPlaylistFavoritos(String nome, List<Musica> maisOuvidas) {
        if (plano == PlanoSubscricao.FREE) {
            System.out.println("Utilizadores FREE não têm acesso a playlist de favoritos.");
            return;
        }

        PlaylistFavoritos nova = new PlaylistFavoritos(nome, maisOuvidas);
        playlists.add(nova);
    }

    public void criarPlaylistPorGeneroDuracao(String nome, List<Musica> todas, String genero, int duracaoMax) {
    PlaylistPorGeneroDuracao nova = PlaylistPorGeneroDuracao.criarAuto(nome, todas, genero, duracaoMax);
    playlists.add(nova);
}


    public List<Musica> getFavoritos() {
        return new ArrayList<>(favoritos);
    }

    public Playlist getPlaylistFavoritos() {
        for (Playlist p : playlists) {
            if (p.getNome().equalsIgnoreCase("Favoritos")) {
                return p;
            }
        }
        return null;
    }

    public void adicionarAFavoritos(Musica musica) {
        if (!favoritos.contains(musica)) {
            favoritos.add(musica);

            Playlist favoritosPlaylist = getPlaylistFavoritos();
            if (favoritosPlaylist == null) {
                favoritosPlaylist = new PlaylistFavoritos("Favoritos", new ArrayList<>());
                playlists.add(favoritosPlaylist);
            }
            favoritosPlaylist.adicionarMusica(musica);
        }
    }

    public void criarPlaylistAleatoria(String nome, int tamanho) {
        List<Musica> todas = module.SpotifUM.getMusicas();
        PlaylistAleatoria nova = new PlaylistAleatoria(nome, todas, tamanho);
        playlists.add(nova);
    }

    public Playlist getPlaylistPorNome(String nome) {
        for (Playlist p : playlists) {
            if (p.getNome().equalsIgnoreCase(nome)) {
                return p;
            }
        }
        return null;
    }

    public int getMusicasOuvidas() {
        return this.totalMusicasOuvidas;
    }
    

    @Override
    public String toString() {
        return "\nUtilizador: " + nome +
                "\nEmail: " + email +
                "\nMorada: " + morada +
                "\nPlano: " + plano +
                "\nPontos: " + pontos +
                "\nTotal Músicas Ouvidas: " + totalMusicasOuvidas +
                "\nPlaylists: " + playlists.size() +
                "\nBiblioteca de Álbuns: " + biblioteca.size();
    }

    @Override
    public Utilizador clone() {
        Utilizador u = new Utilizador(this.nome, this.email, this.morada, this.plano);
        u.pontos = this.pontos;
        u.totalMusicasOuvidas = this.totalMusicasOuvidas;

        for (Album a : this.biblioteca) {
            u.biblioteca.add(a.clone());
        }

        for (Musica m : this.favoritos) {
            u.favoritos.add(m); 
        }

        return u;
    }
}

