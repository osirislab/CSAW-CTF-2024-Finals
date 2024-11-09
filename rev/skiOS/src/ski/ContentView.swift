//
//  ContentView.swift
//  ski
//
//  Created by OSIRIS on 11/1/24.
//

import SwiftUI

struct ContentView: View {
    @State private var username = "OSIRIS"
    @State private var password = ""
    @State private var wrongUsername = 0
    @State private var wrongPassword = 0
    @State private var showingLoginScreen = false

    var body: some View {
        NavigationStack {
            Text("Login")
                .font(.largeTitle)
                .bold()
                .padding()
            TextField("Username", text: $username)
                .padding()
                .frame(width: 300, height: 50)
                .background(Color.black.opacity(0.05))
                .cornerRadius(10)
                .border(.red, width: CGFloat(wrongUsername))
            SecureField("Password", text: $password)
                .padding()
                .frame(width: 300, height: 50)
                .background(Color.black.opacity(0.05))
                .cornerRadius(10)
                .border(.red, width: CGFloat(wrongPassword))
            
            Button("Login") {
                authenticateUser(username: username, password: password)
            }
            .foregroundColor(.white)
            .frame(width: 300, height: 50)
            .background(Color.blue)
            .cornerRadius(10)

            NavigationLink(destination: Text("Congratulations, you solved the challenge!"), isActive: $showingLoginScreen) {
                EmptyView()
            }
        }
    }

    func authenticateUser(username: String, password: String) {
        if username == "OSIRIS" {
            wrongUsername = 0
            print("flag{bruh stringing the file isn't the strat}")
            if password == "csaw_ctf{" + getPassword() + "}" {
                wrongPassword = 0
                showingLoginScreen = true 
            } else {
                wrongPassword = 2
            }
        } else {
            wrongUsername = 2
        }
    }

    func getPassword() -> String {
        let part1 = "purple"
        let part2 = "nxhtrnslfuuqj"
        return part1 + helper1(part:part2)
    }

    func helper1(part: String) -> String {
        var result_chars : [Character] = []

        for char in part {
            let shift = char.asciiValue! - 5
            result_chars.append(Character(UnicodeScalar(shift)))
        }

        return String(result_chars)
    }
}

#Preview {
    ContentView()
}