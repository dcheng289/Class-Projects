package messages;

import javax.swing.Timer;

public class TimerMessage implements Message {
	
	Timer t;
	public TimerMessage(Timer t) {
		this.t = t;
	}
	
	public Timer getTimer() {
		return t;
	}

}
