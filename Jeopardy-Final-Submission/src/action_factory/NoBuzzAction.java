package action_factory;

import frames.MainGUINetworked;
import game_logic.ServerGameData;
import messages.Message;
import messages.NoBuzzMessage;
import other_gui.QuestionGUIElementNetworked;
import other_gui.TeamGUIComponents;
import server.Client;

public class NoBuzzAction extends Action {

	private QuestionGUIElementNetworked currentQuestion;
	private TeamGUIComponents currentTeam;
	
	@Override
	public void executeAction(MainGUINetworked mainGUI, ServerGameData gameData, Message message, Client client) {
		
		NoBuzzMessage resetTimerMessage = (NoBuzzMessage) message;
		currentQuestion = client.getCurrentQuestion();
		currentQuestion.stopTimer();
		currentTeam = currentQuestion.getCurrentTeam();
		gameData.setNextTurn(gameData.nextTurn(currentQuestion.getOriginalTeam()));
		
		//if (resetTimerMessage.getUpdate()) {
			mainGUI.addUpdate("No one answered the question correctly. The correct answer was: "+currentQuestion.getAnswer());
		//}
		//else {
			//add update to Game Progress, determine whether the game board buttons should be enabled or disabled, and change the main panel
			mainGUI.addUpdate("It is "+gameData.getCurrentTeam().getTeamName()+"'s turn to choose a question.");
			if (gameData.getCurrentTeam().getTeamIndex() != client.getTeamIndex()) 
				mainGUI.disableAllButtons();
			mainGUI.showMainPanel();
			mainGUI.startTimer();
		//}
		
	}

}
