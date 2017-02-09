package action_factory;

import frames.MainGUINetworked;
import game_logic.ServerGameData;
import messages.Message;
import messages.QuestionExpireMessage;
import other_gui.QuestionGUIElementNetworked;
import other_gui.TeamGUIComponents;
import server.Client;

public class QuestionExpireAction extends Action {
	
	private QuestionGUIElementNetworked currentQuestion;
	private TeamGUIComponents currentTeam;

	public void executeAction(MainGUINetworked mainGUI, ServerGameData gameData, Message message,
			Client client) {
		QuestionExpireMessage resetTimerMessage = (QuestionExpireMessage) message;
		currentQuestion = client.getCurrentQuestion();
		currentTeam = currentQuestion.getCurrentTeam();
		
//		if (!currentQuestion.getIncorrectAnswer()) {
		currentTeam.deductPoints(currentQuestion.getPointValue());
		mainGUI.addUpdate(currentTeam.getTeamName()+" answered incorrectly. $"+currentQuestion.getPointValue()+" will be deducted from their total. ");
//		}
			
		// set buzz in period
			if (currentQuestion.questionDone()){
				//set the next turn to be the team in clockwise order from original team, add an update, and see whether it is time for FJ
				gameData.setNextTurn(gameData.nextTurn(currentQuestion.getOriginalTeam()));
				mainGUI.addUpdate("No one answered the question correctly. The correct answer was: "+currentQuestion.getAnswer());
			}
			//if question has not been attempted by everyone, go to a buzz in period
			else{
				currentQuestion.setBuzzInPeriod();
				mainGUI.addUpdate("Buzz in to answer!");
			}

	}

}
