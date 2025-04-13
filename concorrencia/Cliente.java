package concorrencia;

public class Cliente extends Thread {
    private Pessoa pessoa;
    private Barbearia barbearia;

    public Cliente(int id, Barbearia barbearia) {
        this.pessoa = new Pessoa(id);
        this.barbearia = barbearia;
    }

    public int getID() {
        return pessoa.getID();
    }

    @Override
    public void run() {
        while (true) {
            boolean cortou = barbearia.cortaCabelo(this);
            if (!cortou) {
                System.out.println("Cliente " + getID() + " tentou entrar na barbearia, mas está lotada... indo dar uma voltinha");
            }
            try {
                Thread.sleep((long) (Math.random() * 2000 + 3000)); // 3-5 segundos
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}