package action_factory;

import frames.MainGUINetworked;
import game_logic.ServerGameData;
import messages.Message;
import messages.QuestionClickedMessage;
import other_gui.QuestionGUIElementNetworked;
import server.Client;

public class QuestionClickedAction extends Action{

	public void executeAction(MainGUINetworked mainGUI, ServerGameData gameData, Message message,
			Client client) {
		QuestionClickedMessage qClickedMessage = (QuestionClickedMessage) message;
		//set the current question on the client using the indices provided in the message
		client.setCurrentQuestion(qClickedMessage.getX(), qClickedMessage.getY());
		//get the new current question and set the original team that chose it
		QuestionGUIElementNetworked newCurrentQuestion = client.getCurrentQuestion();
		newCurrentQuestion.setOriginalTeam(gameData.getCurrentTeam().getTeamIndex(), gameData);
		//change to question panel, update number of chosen questions in game data, and set this question to asked
		mainGUI.changePanel(newCurrentQuestion.getQuestionPanel());
		gameData.updateNumberOfChosenQuestions();
		newCurrentQuestion.startTimer();
		newCurrentQuestion.setAsked();
	}

}