package action_factory;

import frames.MainGUINetworked;
import game_logic.ServerGameData;
import messages.Message;
import messages.TimerMessage;
import server.Client;

public class StopTimerAction extends Action{

	@Override
	public void executeAction(MainGUINetworked mainGUI, ServerGameData gameData, Message message, Client client) {

		TimerMessage timerMessage = (TimerMessage) message;
		
		// stop all timers
		mainGUI.stopTimer();
		//client.getCurrentQuestion().stopTimer();
		
	}

}

