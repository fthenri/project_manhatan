package concorrencia;

public class Barbeiro extends Thread {
    private Pessoa pessoa;
    private Barbearia barbearia;

    public Barbeiro(int id, Barbearia barbearia) {
        this.pessoa = new Pessoa(id);
        this.barbearia = barbearia;
    }

    public int getID() {
        return pessoa.getID();
    }

    @Override
    public void run() {
        while (true) {
            Cliente cliente = barbearia.proximoCliente();
            if (cliente != null) {
                System.out.println("Cliente " + cliente.getID() + " cortando cabelo com Barbeiro " + getID());
                try {
                    Thread.sleep((long) (Math.random() * 2000 + 1000));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                barbearia.corteTerminado(cliente);
            }
        }
    }
}