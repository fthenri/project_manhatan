package concorrencia;

import java.util.LinkedList;
import java.util.Queue;

public class Barbearia {
    private int nBarbeiros;
    private int mCadeirasEspera;
    private Queue<Cliente> filaEspera;
    private Cliente[] clientesEmAtendimento;
    private boolean[] barbeirosDormindo;

    public Barbearia(int nBarbeiros, int mCadeirasEspera) {
        this.nBarbeiros = nBarbeiros;
        this.mCadeirasEspera = mCadeirasEspera;
        this.filaEspera = new LinkedList<>();
        this.clientesEmAtendimento = new Cliente[nBarbeiros];
        this.barbeirosDormindo = new boolean[nBarbeiros];
        for (int i = 0; i < nBarbeiros; i++) {
            barbeirosDormindo[i] = true;
        }
    }

    public synchronized boolean cortaCabelo(Cliente cliente) {
        if (filaEspera.size() >= mCadeirasEspera) {
            return false;
        }
        filaEspera.add(cliente);
        System.out.println("Cliente " + cliente.getID() + " esperando corte...");
        notifyAll(); 
        return true;
    }

    public synchronized Cliente proximoCliente() {
        while (filaEspera.isEmpty()) {
            int barbeiroId = Integer.parseInt(Thread.currentThread().getName().split("-")[1]) - 1;
            barbeirosDormindo[barbeiroId] = true;
            System.out.println("Barbeiro " + (barbeiroId + 1) + " indo dormir... não há clientes");
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            barbeirosDormindo[barbeiroId] = false;
            System.out.println("Barbeiro " + (barbeiroId + 1) + " acordou! Começando os trabalhos!");
        }
        Cliente cliente = filaEspera.poll();
        int barbeiroId = Integer.parseInt(Thread.currentThread().getName().split("-")[1]) - 1;
        clientesEmAtendimento[barbeiroId] = cliente;
        return cliente;
    }

    public synchronized void corteTerminado(Cliente cliente) {
        int barbeiroId = Integer.parseInt(Thread.currentThread().getName().split("-")[1]) - 1;
        clientesEmAtendimento[barbeiroId] = null;
        System.out.println("Cliente " + cliente.getID() + " terminou o corte... saindo da barbearia!");
    }
}