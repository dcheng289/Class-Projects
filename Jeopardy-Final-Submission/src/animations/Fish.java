package animations;

import java.awt.Dimension;
import java.awt.Image;

import javax.swing.ImageIcon;
import javax.swing.JLabel;

public class Fish implements Runnable  {

	private JLabel fishLabel;
	private ImageIcon[] fishIcon;
	private int totalImages = 8, currentImage = 0;
	Thread fishThread;
	
	public Fish() {
		fishLabel = new JLabel();
		fishLabel.setPreferredSize(new Dimension(60, 60));
		fishIcon = new ImageIcon[totalImages];
		for (int i = 0; i < fishIcon.length; ++i ) {
			fishIcon[i] = new ImageIcon(new ImageIcon( "images/waitingAnimation/frame_" + i + "_delay-0.1s.jpg" ).getImage().getScaledInstance(65, 65, Image.SCALE_DEFAULT));
		}
		this.start();
	}
	 
    public final void start() {
        if (fishThread == null){
        	fishThread = new Thread (this);
            fishThread.start();
        }
    } 

	@Override
	public void run() {
        while (fishThread != null){
            try{
                Thread.sleep(100);
                currentImage++;
                fishLabel.setIcon(fishIcon[currentImage]);
            }
            catch (InterruptedException e) { }
            if (this.isDone())  {
            	
            }
        }
	}

    public boolean isDone() {
        boolean buf = false;
  
        if (currentImage == totalImages-1) {
            buf = true;
            currentImage = 0;
        }
    
        return buf;
    }
    
    public JLabel getLabel() {
    	return fishLabel;
    }

    
    public void setVisible(boolean b) {
    	fishLabel.setVisible(b);
    }
    
   /* public static void main(String[] args) {
    	fish fish = new fish();
    	JFrame frame = new JFrame("Arf");
    	//frame.setSize(1086, 1200);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(fish.getLabel());
        frame.setVisible(true);
    }*/

}