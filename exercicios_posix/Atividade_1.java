package infraDeSo.exercisesposix;

import java.util.EmptyStackException;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Atividade1 {
	int itens[] = {1, 2, 3, 4, 5};
	int buffer[] = new int[5];
	static int compartilhado = 0;
	static Atividade1 shared = new Atividade1();
	
	static Semaphore empty = new Semaphore(5);
	static Semaphore ocup = new Semaphore(0);
	static Semaphore turn = new Semaphore(1);
    static private final Lock lock = new ReentrantLock();

	
	static class Producer extends Thread
	{

		@Override
		public void run() {
			try {
			// TODO Auto-generated method stub
				for (int i = 0; i < 5; i++)
				{
					turn.acquire();
					empty.acquire();
						lock.lock();
							System.out.println("Produzindo.. ");
							Atividade1.shared.buffer[compartilhado] = Atividade1.shared.itens[compartilhado];
							compartilhado++;
						lock.unlock();
					ocup.release();

				}
			}
			
			catch (Exception e) {
				e.printStackTrace();// TODO: handle exception
			}
		}
	}

	static class Consumer extends Thread
	{
		@Override
		public void run() {
			// TODO Auto-generated method stub
			try {
			// TODO Auto-generated method stub
				for (int i = 0; i < 5; i++)
				{
					ocup.acquire();
						lock.lock();
						System.out.println("Consumindo..") ;
						compartilhado--;
						Atividade1.shared.buffer[compartilhado] = 0;
						lock.unlock();
					empty.release();
					turn.release();
				}
			}
			
			catch (Exception e) {
				e.printStackTrace();// TODO: handle exception
			}
		}
	}
	
	
	public static void main(String[] args) {
			Producer producer = new Producer();
			Consumer consumer = new Consumer();
			producer.start();
			consumer.start();
	}
}
