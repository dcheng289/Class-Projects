package messages;

public class NoBuzzMessage implements Message {

	private boolean status;
	
	public NoBuzzMessage(boolean status) {
		this.status = status;
	}
	
	public boolean getUpdate() {
		return status;
	}
}
