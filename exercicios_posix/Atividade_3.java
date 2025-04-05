import java.util.concurrent.Semaphore;

public class Atividade_3 {

    static int readCount = 0;
    static int writeCount = 0;

    static final Semaphore mutexRead = new Semaphore(1);
    static final Semaphore mutexWrite = new Semaphore(1);
    static final Semaphore rw = new Semaphore(1);
    static final Semaphore tryRead = new Semaphore(1);

    static class Leitor extends Thread {
        private final int id;

        public Leitor(int id) {
            this.id = id;
        }

        @Override
        public void run() {
            try {
                tryRead.acquire();
                mutexRead.acquire();
                readCount++;
                if (readCount == 1) {
                    rw.acquire(); // primeiro leitor bloqueia escritores
                }
                mutexRead.release();
                tryRead.release();

                System.out.println("Leitor " + id + " está lendo...");
                Thread.sleep(1000);
                System.out.println("Leitor " + id + " terminou de ler.");

                mutexRead.acquire();
                readCount--;
                if (readCount == 0) {
                    rw.release(); // último leitor libera escritores
                }
                mutexRead.release();

            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    static class Escritor extends Thread {
        private final int id;

        public Escritor(int id) {
            this.id = id;
        }

        @Override
        public void run() {
            try {
                mutexWrite.acquire();
                writeCount++;
                if (writeCount == 1) {
                    tryRead.acquire(); // primeiro escritor bloqueia novos leitores
                }
                mutexWrite.release();

                rw.acquire(); // espera leitores/escritores anteriores

                System.out.println("Escritor " + id + " está escrevendo...");
                Thread.sleep(2000);
                System.out.println("Escritor " + id + " terminou de escrever.");

                rw.release();

                mutexWrite.acquire();
                writeCount--;
                if (writeCount == 0) {
                    tryRead.release(); // último escritor libera leitores
                }
                mutexWrite.release();

            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        Leitor[] leitores = new Leitor[5];
        Escritor[] escritores = new Escritor[3];

        // Criando leitores
        for (int i = 0; i < leitores.length; i++) {
            leitores[i] = new Leitor(i + 1);
            leitores[i].start();
            try { Thread.sleep(500); } catch (InterruptedException ignored) {}
        }

        // Criando escritores
        for (int i = 0; i < escritores.length; i++) {
            escritores[i] = new Escritor(i + 1);
            escritores[i].start();
            try { Thread.sleep(1000); } catch (InterruptedException ignored) {}
        }

        // Esperando todos terminarem
        for (Leitor l : leitores) {
            try { l.join(); } catch (InterruptedException ignored) {}
        }
        for (Escritor e : escritores) {
            try { e.join(); } catch (InterruptedException ignored) {}
        }

        System.out.println("Todos os leitores e escritores terminaram.");
    }
}
