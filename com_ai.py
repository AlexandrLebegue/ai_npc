from openai import OpenAI


# BASIC_MODEL     = "gpt-3.5-turbo"
# BASIC_URL       = "https://api.openai.com/v1/chat/completions"
# BASIC_API_KEY   = ""

BASIC_MODEL     = "TheBloke/Mistral-7B-Instruct-v0.2-GGUF"
BASIC_URL       = "http://localhost:1234/v1"
BASIC_API_KEY   = "lm-studio"

class AiAgent:
    
    BASIC_MESSAGE =  {"role": "assistant", "content": ""}

    def __init__(self, base_url=BASIC_URL, api_key=BASIC_API_KEY, model= BASIC_MODEL):
        """
        Initialize an AI agent with a given OpenAI client and model.
        
        Args:
            base_url (str): The base URL of the OpenAI API server.
            api_key (str): The API key for the OpenAI API.
            model (str): The name of the model to use for generating completions.
        """
        self.client = OpenAI(base_url=base_url, api_key=api_key)
        self.model = model
        self.history = [
            #{"role": "system", "content": "You are an intelligent assistant. You always provide well-reasoned answers that are both correct and helpful."},
            {"role": "user", "content": "Below is an instruction that describes a task. Write a response that appropriately completes the request."},
        ]

    def chat(self):
        """
        Start a conversation with the AI model in an infinite loop.
        """
        while True:
            completion = self.client.chat.completions.create(
                model=self.model,
                messages=self.history,
                temperature=0.7,
                stream=True,
            )

            new_message = self.BASIC_MESSAGE
            
            for chunk in completion:
                if chunk.choices[0].delta.content:
                    print(chunk.choices[0].delta.content, end="", flush=True)
                    new_message["content"] += chunk.choices[0].delta.content

            self.history.append(new_message)
            
            print()
            self.history.append({"role": "user", "content": input("> ")})
            
    
    def send_message(self, text, temp = 0.2):
        """
        Send a user's message to the AI model and return its response.
        
        Args:
            message (str): The user's message.
            
        Returns:
            str: The AI model's response.
        """

        text = text.replace("\n", " ")
        
        self.history.append({"role": "user", "content": text})


        completion = self.client.chat.completions.create(
                model=self.model,
                messages=self.history,
                temperature=temp,
                # top_p=0.8,
                frequency_penalty= 0.4,
                # presence_penalty= 0.2,
                stream=True,
            )
        
        new_message = self.BASIC_MESSAGE
        new_message["content"] = ""
        for chunk in completion:
                if chunk.choices[0].delta.content:
                    #print(chunk.choices[0].delta.content, end="", flush=True)
                    new_message["content"] += chunk.choices[0].delta.content
        
        
        return new_message["content"]


if __name__ == "__main__":
    oui = AiAgent()
    print(oui.send_message("Bonjourjij uyuhiç"))

