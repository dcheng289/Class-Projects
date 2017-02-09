package animations;

import java.awt.Dimension;
import java.awt.Image;

import javax.swing.ImageIcon;
import javax.swing.JLabel;

public class Clock implements Runnable  {

	private JLabel clockLabel;
	private ImageIcon[] clockIcon;
	private int totalImages = 143, currentImage = 0;
	Thread clockThread;
	
	public Clock() {
		clockLabel = new JLabel();
		clockLabel.setPreferredSize(new Dimension(60, 60));
		clockIcon = new ImageIcon[totalImages];
		for (int i = 0; i < clockIcon.length; ++i ) {
			clockIcon[i] = new ImageIcon(new ImageIcon( "images/clockAnimation/frame_" + i + "_delay-0.06s.jpg" ).getImage().getScaledInstance(65, 65, Image.SCALE_DEFAULT));
		}
		this.start();
	}
	 
    public final void start() {
        if (clockThread == null){
        	clockThread = new Thread (this);
            clockThread.start();
        }
    } 

	@Override
	public void run() {
        while (clockThread != null){
            try{
                Thread.sleep(60);
                currentImage++;
                clockLabel.setIcon(clockIcon[currentImage]);
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
    	return clockLabel;
    }

    
    public void setVisible(boolean b) {
    	clockLabel.setVisible(b);
    }


}