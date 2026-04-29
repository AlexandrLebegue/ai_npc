import json
import logging
import com_ai
import ruamel.yaml
import streamlit as st

CONTEXT = """
CONTEXT :
We are in an apocalypse world where a nuclear war killed mainly all life in the earth. Only few survivors remains.

PROMPT :
"""


class Location:
    def __init__(self):
        self.City = ''
        self.Address = ''


class Person:

    def __init__(self):
        self.Name = ''
        self.FamilyName = ''
        self.Age = 0
        self.Gender = ''
        self.Location = Location()
        self.Ethnicity = ''
        self.Height = 0  # in inches
        self.Weight = 0  # in pounds
        self.EyeColor = ''
        self.HairColor = ''
        self.SkinTone = ''
        self.Nationality = ''
        self.Occupation = ''
        self.EducationLevel = ''
        self.MaritalStatus = ''
        self.Religion = ''
        self.HealthConditions = []

    def update_from_dict(self, data):
        for key, value in data.items():
            if key == 'Location' and isinstance(value, dict):
                self.Location.City = value.get('City', '')
                self.Location.Address = value.get('Address', '')
            elif hasattr(self, key):
                setattr(self, key, value)

    def update_from_json(self, data):
        data = json.loads(data)
        self.update_from_dict(data)

    def save_person(self, filename):
        with open(filename, 'a+') as json_file:
            json_file.write(json.dumps(self.__dict__, ensure_ascii=False) + ",\n")

    def __str__(self):
        return f"""
        Name: {self.Name}
        FamilyName: {self.FamilyName}
        Age: {self.Age}
        Gender: {self.Gender}
        Location: {self.Location.City}, {self.Location.Address}
        Ethnicity: {self.Ethnicity}
        Height (inches): {self.Height}
        Weight (pounds): {self.Weight}
        Eye Color: {self.EyeColor}
        Hair Color: {self.HairColor}
        Skin Tone: {self.SkinTone}
        Nationality: {self.Nationality}
        Occupation: {self.Occupation}
        Education Level: {self.EducationLevel}
        Marital Status: {self.MaritalStatus}
        Religion: {self.Religion}
        Health Conditions: {', '.join(self.HealthConditions)}
        """


class Npc(Person):

    def __init__(self, context=None):
        self.context = context or CONTEXT
        self.agent = com_ai.AiAgent()
        self.quest = None
        super().__init__()

    def update_from_dict(self, data):
        super().update_from_dict(data)

    def saluate(self):
        prepared_prompt = self.context
        prepared_prompt += f"You are playing the role a human. Give response as you talk to someone. Do not repeat already said information. Information below is your identity \n {self.__str__()} \n"
        prepared_prompt += """Someone approaches you in the street. Open the discussion with a concise response.
        You don't need to say all about you to a stranger. Go to the essential. Express your nationality with the response.
        Do not give your adress it could be dangerous ! : """
        return self.agent.send_message(prepared_prompt, temp=0.9)

    def give_a_quest(self):
        prepared_prompt = self.context
        prepared_prompt += f"""
You are playing the role of {self.Name} {self.FamilyName}, a {self.Occupation} from {self.Location.City}.
You need help and want to give a quest to the player.
Based on your identity and the post-apocalyptic context, invent a realistic quest.
Be concise. Output must be exactly this JSON form, nothing else:
{{
    "title": "",
    "description": "",
    "objective": "",
    "reward": "",
    "difficulty": ""
}}
"""
        output = ""
        try:
            output = self.agent.send_message(prepared_prompt, temp=0.8)
            start = output.find('{')
            end = output.rfind('}') + 1
            quest_data = json.loads(output[start:end])
            self.quest = quest_data
            return quest_data
        except Exception as e:
            logging.error(f"Quest generation failed: {e}\n{output}")
            fallback = {
                "title": "Aide-moi",
                "description": "Le PNJ a besoin d'aide mais ne peut pas l'exprimer clairement.",
                "objective": "Parler au PNJ",
                "reward": "Gratitude",
                "difficulty": "Facile"
            }
            self.quest = fallback
            return fallback

    def talk(self, input):
        prepared_prompt = f"""The player ask you this:
        ```player
        {input}
        ```

        Give a correctly response according to your identity. Again, be concise. Do not add any special characters, only text :"""
        return self.agent.send_message(prepared_prompt, temp=0.9)

    def __str__(self):
        return super().__str__()


class Player(Person):

    def __init__(self, context=None):
        self.context = context or CONTEXT
        self.agent = com_ai.AiAgent()
        self.current_responses = []
        init_prompt = """
        Your are the main character of this adventure. Your are the player. A human is controlling you. Your role is to facilitate the interaction between the player and others npc.
        Be concise. Write to the first person. Write only if the prompt ask you to write. Do not respond to this.
        """
        self.agent.send_message(init_prompt, temp=0.2)
        super().__init__()

    def update_from_dict(self, data):
        super().update_from_dict(data)

    def talk(self, input):
        prepared_prompt = f"""The npc answer this input =
        ```npc
        {input}
        ```
        This is not you. This response come from the npc.
        Prepare 3 response to answer to this. Be consise and stay roleplay. The output must be this form :

        * 'first response'
        * 'second response'
        * 'third response'

        """
        output = self.agent.send_message(prepared_prompt, temp=0.2)
        self.current_responses = output.split("*")
        return output

    def choose_response(self, number):
        return self.current_responses[number]


def generate_persons_with_ai(nb_to_generate=1, context="", file_to_save=None):

    def generate_one_person_with_ai(agent_to_talk, context):
        prompt = context
        prompt += """
        Invent only one person. It must be different from the precedent prompted one. The gender must be male, or non-binary. The name must be different. Be concise. Fill all his personal informations in the given output. The output must be exactly this form :
        {
            "Gender": "",
            "Age": ,
            "Name": "",
            "FamilyName": "",
            "Location": {"City": "", "Address": ""},
            "Ethnicity": "",
            "Height": ,
            "Weight": ,
            "EyeColor": "",
            "HairColor": "",
            "SkinTone": "",
            "Nationality": "",
            "Occupation": "",
            "EducationLevel": "",
            "MaritalStatus": "",
            "Religion": "",
            "HealthConditions": [""]
        }
        """
        json_data = None
        try:
            json_data = agent_to_talk.send_message(prompt, temp=0.7)
            generated_person = Person()
            generated_person.update_from_json(json_data)
        except Exception as e:
            logging.error("ERREUR GENERATION prompt = " + str(e) + "\n\n" + str(json_data))
            logging.error("Trying again ... \n\n")
            return generate_one_person_with_ai(agent_to_talk, context)
        return generated_person

    agent_to_talk = com_ai.AiAgent()
    list_of_persons = []
    for i in range(0, nb_to_generate):
        person = generate_one_person_with_ai(agent_to_talk, context)
        print(f"\n\nNUMBER {i} : \n", person)
        if file_to_save is not None:
            person.save_person(file_to_save)
        list_of_persons.append(person)

    return list_of_persons


def init_env():
    default_player = {
        "Gender": "Male",
        "Age": 35,
        "Name": "Alexandre",
        "FamilyName": "Lebegue",
        "Location": {
            "City": "Paris",
            "Address": "123 Main St"
        },
        "Ethnicity": "French",
        "Height": 175,
        "Weight": 70,
        "EyeColor": "Brown",
        "HairColor": "Black",
        "SkinTone": "Mediterranean",
        "Nationality": "American",
        "Occupation": "Software Engineer",
        "EducationLevel": "Master's Degree",
        "MaritalStatus": "Single",
        "Religion": "Agnostic",
        "HealthConditions": []
    }

    default_npc = {
        "Gender": "Male",
        "Age": 35,
        "Name": "Leo Torres",
        "FamilyName": "Ramirez",
        "Location": {
            "City": "San Francisco",
            "Address": "123 Main St"
        },
        "Ethnicity": "Hispanic",
        "Height": 175,
        "Weight": 75,
        "EyeColor": "Brown",
        "HairColor": "Black",
        "SkinTone": "Mediterranean",
        "Nationality": "American",
        "Occupation": "Software Engineer",
        "EducationLevel": "Master's Degree",
        "MaritalStatus": "Single",
        "Religion": "Agnostic",
        "HealthConditions": []
    }

    ctx = st.session_state.get('context', CONTEXT)
    st.session_state['player'] = Player(ctx)
    st.session_state['player'].update_from_dict(default_player)
    st.session_state['npc'] = Npc(ctx)
    st.session_state['npc'].update_from_dict(default_npc)
    st.session_state['is_initialised'] = True
    st.session_state['npc_output'] = st.session_state['npc'].saluate()
    st.session_state['npc_has_talked'] = True
    st.session_state['npc_responses'] = [st.session_state['npc_output']]
    st.session_state['player_responses'] = []
    st.session_state['current_quest'] = None
    st.session_state['last_npc_file_id'] = None
    st.session_state['last_player_file_id'] = None


def player_respond(index):
    st.session_state['player_response'] = st.session_state['player'].choose_response(index)
    st.session_state['npc_output'] = st.session_state['npc'].talk(st.session_state['player_response'])
    st.session_state['npc_responses'].append(st.session_state['npc_output'])
    st.session_state['player_responses'].append(st.session_state['player_response'])
    st.session_state['npc_has_talked'] = True


def run_streamlit():
    if st.session_state.get('context') is None:
        st.session_state['context'] = CONTEXT

    if st.session_state.get('is_initialised') is None:
        init_env()

    with st.sidebar:
        with st.expander("Configuration ⚙️", True):
            st.session_state['context'] = st.text_area(
                "Définition du contexte historique 🪐",
                st.session_state['context'],
                height=200
            )
            com_ai.BASIC_MODEL = st.text_input("AI model 🤖", value=com_ai.BASIC_MODEL)
            com_ai.BASIC_URL = st.text_input("url 🌍", value=com_ai.BASIC_URL)
            com_ai.BASIC_API_KEY = st.text_input("api-key 🔑", value=com_ai.BASIC_API_KEY)

            if st.button("Reset", type="primary"):
                init_env()

        with st.expander("Votre personnage 🧔‍♂️", True):
            st.session_state['player'].Name = st.text_input("Nom", value=st.session_state['player'].Name)
            st.session_state['player'].FamilyName = st.text_input("Prénom", value=st.session_state['player'].FamilyName)
            st.session_state['player'].Age = st.number_input("Age", min_value=0, max_value=100, step=1, value=st.session_state['player'].Age)
            player_file = st.file_uploader("Charger à partir d'un fichier Json", key="player_upload")
            if player_file is not None and player_file.file_id != st.session_state.get('last_player_file_id'):
                st.session_state['last_player_file_id'] = player_file.file_id
                try:
                    data = json.loads(player_file.read().decode("utf-8"))
                    st.session_state['player'].update_from_dict(data)
                    st.success(f"Joueur chargé : {st.session_state['player'].Name}")
                except (json.JSONDecodeError, UnicodeDecodeError) as e:
                    st.error(f"Fichier JSON invalide : {e}")

        with st.expander("NPC 🤖", True):
            st.session_state['npc'].Name = st.text_input("Nom", value=st.session_state['npc'].Name, key="npc_name")
            st.session_state['npc'].FamilyName = st.text_input("Prénom", value=st.session_state['npc'].FamilyName, key="npc_family")
            st.session_state['npc'].Age = st.number_input("Age", min_value=0, max_value=100, step=1, value=st.session_state['npc'].Age, key="npc_age")
            npc_file = st.file_uploader("Charger à partir d'un fichier Json", key="npc_upload")
            if npc_file is not None and npc_file.file_id != st.session_state.get('last_npc_file_id'):
                st.session_state['last_npc_file_id'] = npc_file.file_id
                try:
                    data = json.loads(npc_file.read().decode("utf-8"))
                    st.session_state['npc'].update_from_dict(data)
                    new_greeting = st.session_state['npc'].saluate()
                    st.session_state['npc_output'] = new_greeting
                    st.session_state['npc_responses'] = [new_greeting]
                    st.session_state['player_responses'] = []
                    st.session_state['current_quest'] = None
                    st.success(f"NPC chargé : {st.session_state['npc'].Name}")
                except (json.JSONDecodeError, UnicodeDecodeError) as e:
                    st.error(f"Fichier JSON invalide : {e}")

    st.write("# NPC talk 3000 🤖")

    with st.expander(st.session_state['npc'].Name, True):
        for response in st.session_state['npc_responses']:
            with st.chat_message("assistant"):
                st.write(response)

    if st.session_state['npc_has_talked']:
        st.session_state['player'].talk(st.session_state['npc_output'])

    with st.expander("Vous", True):
        for response in st.session_state['player_responses']:
            with st.chat_message("user"):
                st.write(response)

        for index, response in enumerate(st.session_state['player'].current_responses):
            response = response.strip().strip("'")
            if not response:
                continue
            st.button(response, key=str(index), on_click=player_respond, args=(index,))

    st.divider()

    if st.button("Donner une quête 📜"):
        with st.spinner("Génération de la quête..."):
            quest = st.session_state['npc'].give_a_quest()
            st.session_state['current_quest'] = quest

    if st.session_state.get('current_quest'):
        with st.expander("Quête en cours 📜", True):
            q = st.session_state['current_quest']
            st.markdown(f"**{q.get('title', '')}**")
            st.write(q.get('description', ''))
            st.write(f"🎯 Objectif : {q.get('objective', '')}")
            st.write(f"🏆 Récompense : {q.get('reward', '')}")
            st.write(f"⚔️ Difficulté : {q.get('difficulty', '')}")


if __name__ == "__main__":
    run_streamlit()
