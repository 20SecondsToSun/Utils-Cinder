#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "SocialShare.h"
#include "Utils.h"
#include "Curl.h"
#include "twitcurl.h"

namespace twDefaults
{
	const std::string TW_API_KEY    = "dlNLtfhvXIdVQLr6s55plKJzb";
	const std::string TW_API_SECRET = "XBVOiDlLJ7kLNjqy1a29lR3HUaVkLDtgGqa1snQTg9DeBesrP8";

	const std::string TOKEN_KEY_COOKIE    = "twitterClient_token_key_cookie.txt";
	const std::string TOKEN_SECRET_COOKIE = "twitterClient_token_secret_cookie.txt"; 
}

namespace twParams
{	
	const std::string STATUS_DEFAULT						= "#ТойотаНастроение";
}


class Twitter: public SocShare
{
	public:
		Twitter(){ isAuthFlowComplete = false;  twitterAuthURL = "https://twitter.com";};

		bool							init(std::string login, std::string password);
		bool							postTextTweet(string status);
		bool							postPhotoTweet(std::string status, std::vector<std::string> filesPath);
		bool							postPhotoTweetBase64(std::string status, std::string filesPath);
	
		const char *					getAuthUrl();	
		void							post();
		void							logOut();

	private :	

		bool							isAuthFlowComplete;

		twitCurl						twitterObj;	

		std::shared_ptr<std::thread>	serverThread;

		void							twitterPostThread();

		std::string						twitterAuthURL;		
		std::string						getDefaultStatus();
};