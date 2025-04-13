package concorrencia;

public class Main {
    public static void main(String[] args) {
        if (args.length != 3) {
            System.out.println("Uso: java Main <nBarbeiros> <mCadeirasEspera> <totalClientes>");
            return;
        }

        int nBarbeiros = Integer.parseInt(args[0]);
        int mCadeirasEspera = Integer.parseInt(args[1]);
        int totalClientes = Integer.parseInt(args[2]);

        Barbearia barbearia = new Barbearia(nBarbeiros, mCadeirasEspera);

        for (int i = 1; i <= nBarbeiros; i++) {
            Barbeiro barbeiro = new Barbeiro(i, barbearia);
            barbeiro.setName("Barbeiro-" + i);
            barbeiro.start();
        }

        // Inicia clientes
        for (int i = 1; i <= totalClientes; i++) {
            Cliente cliente = new Cliente(i, barbearia);
            cliente.start();
        }
    }
}