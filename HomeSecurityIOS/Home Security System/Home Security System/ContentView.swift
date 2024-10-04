import SwiftUI

struct ContentView: View {
    var body: some View {
        VStack {
            // Centered Header
            Text("Home Security System")
                .font(.largeTitle)
                .fontWeight(.bold)
                .multilineTextAlignment(.center)
                .padding()
                .frame(maxWidth: .infinity, alignment: .center)

            Spacer()

            // Login Button
            Button(action: {
                // Action for Login button
                print("Login button tapped")
            }) {
                Text("Login")
                    .font(.title2)
                    .fontWeight(.semibold)
                    .padding()
                    .frame(maxWidth: .infinity)
                    .background(
                        RoundedRectangle(cornerRadius: 12)
                            .stroke(Color.blue, lineWidth: 2)
                    )
                    .foregroundColor(.blue)
                    .padding(.horizontal, 40)
            }
            .padding(.bottom, 20)

            // Register Button
            Button(action: {
                // Action for Register button
                print("Register button tapped")
            }) {
                Text("Register")
                    .font(.title2)
                    .fontWeight(.semibold)
                    .padding()
                    .frame(maxWidth: .infinity)
                    .background(
                        RoundedRectangle(cornerRadius: 12)
                            .stroke(Color.green, lineWidth: 2)
                    )
                    .foregroundColor(.green)
                    .padding(.horizontal, 40)
            }

            Spacer()
        }
        .padding()
        .background(Color(.systemGroupedBackground))  // Minimalist background color
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
