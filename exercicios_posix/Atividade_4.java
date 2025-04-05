import java.util.concurrent.locks.*;
import java.util.*;

public class Atividade_4 {

    private final ReentrantLock lock = new ReentrantLock();
    private final Condition podeLer = lock.newCondition();
    private final Condition podeEscrever = lock.newCondition();

    private int leitoresAtivos = 0;
    private int escritoresAtivos = 0;
    private int escritoresEsperando = 0;

    private final List<String> recursoCompartilhado = new ArrayList<>();

    public void comecarLer(String nomeLeitor) throws InterruptedException {
        lock.lock();
        try {
            while (escritoresAtivos > 0 || escritoresEsperando > 0) {
                podeLer.await();
            }
            leitoresAtivos++;
        } finally {
            lock.unlock();
        }
    }

    public void terminarLer(String nomeLeitor) {
        lock.lock();
        try {
            leitoresAtivos--;
            if (leitoresAtivos == 0) {
                podeEscrever.signal();
            }
        } finally {
            lock.unlock();
        }
    }

    public void comecarEscrever(String nomeEscritor) throws InterruptedException {
        lock.lock();
        try {
            escritoresEsperando++;
            while (leitoresAtivos > 0 || escritoresAtivos > 0) {
                podeEscrever.await();
            }
            escritoresEsperando--;
            escritoresAtivos++;
        } finally {
            lock.unlock();
        }
    }

    public void terminarEscrever(String nomeEscritor) {
        lock.lock();
        try {
            escritoresAtivos--;
            if (escritoresEsperando > 0) {
                podeEscrever.signal();
            } else {
                podeLer.signalAll();
            }
        } finally {
            lock.unlock();
        }
    }

    public void ler(String nomeLeitor) throws InterruptedException {
        comecarLer(nomeLeitor);
        System.out.println(nomeLeitor + " está lendo: " + recursoCompartilhado);
        Thread.sleep(500);
        terminarLer(nomeLeitor);
    }

    public void escrever(String nomeEscritor, String conteudo) throws InterruptedException {
        comecarEscrever(nomeEscritor);
        System.out.println(nomeEscritor + " está escrevendo: " + conteudo);
        Thread.sleep(500);
        recursoCompartilhado.add(conteudo);
        terminarEscrever(nomeEscritor);
    }

    public static void main(String[] args) {
        LeitoresEscritoresPrioridadeEscritor sistema = new LeitoresEscritoresPrioridadeEscritor();

        for (int i = 1; i <= 3; i++) {
            final String nome = "Leitor-" + i;
            new Thread(() -> {
                while (true) {
                    try {
                        sistema.ler(nome);
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        break;
                    }
                }
            }).start();
        }

        for (int i = 1; i <= 2; i++) {
            final String nome = "Escritor-" + i;
            final String conteudo = "Dado-" + i;
            new Thread(() -> {
                while (true) {
                    try {
                        sistema.escrever(nome, conteudo + "@" + System.currentTimeMillis());
                        Thread.sleep(3000);
                    } catch (InterruptedException e) {
                        break;
                    }
                }
            }).start();
        }
    }
}
