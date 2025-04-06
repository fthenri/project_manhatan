package atividade_2;

import java.util.LinkedList;

class Buffer {
    private final LinkedList<Integer> queue = new LinkedList<>();
    private final int size;

    public Buffer(int size) {
        this.size = size;
    }

    public synchronized void produce(int value) throws InterruptedException {
        while (queue.size() == size) {
            System.out.println("Buffer cheio. Produtor aguardando...");
            wait();
        }

        queue.add(value);
        System.out.println("Produzido: " + value + " | Buffer: " + queue);
        notifyAll();
    }

    public synchronized void consume() throws InterruptedException {
        while (queue.isEmpty()) {
            System.out.println("Buffer vazio. Consumidor aguardando...");
            wait();
        }

        int removed = queue.removeFirst();
        System.out.println("Consumido: " + removed + " | Buffer: " + queue);
        notifyAll();
    }
}

class Producer extends Thread {
    private final Buffer buffer;
    private int counter = 1;

    public Producer(Buffer buffer) {
        this.buffer = buffer;
    }

    public void run() {
        try {
            while (true) {
                buffer.produce(counter += 1);
                Thread.sleep(1000);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

class Consumer extends Thread {
    private final Buffer buffer;

    public Consumer(Buffer buffer) {
        this.buffer = buffer;
    }

    public void run() {
        try {
            while (true) {
                buffer.consume();
                Thread.sleep(2000);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}

public class Atividade_2 {
    public static void main(String[] args) {
        int bufferSize = 5; 
        Buffer buffer = new Buffer(bufferSize);

        new Producer(buffer).start();
        new Consumer(buffer).start();
    }
}
