import { TeamsList } from "./components/TeamsList";
import { GameStatus } from "./components/GameStatus";
import { WebSocketProvider } from "./hooks/useWebSocket";

function App() {
  return (
    <WebSocketProvider>
      <div className="min-h-screen bg-background">
        <div className="container mx-auto py-8 px-4">
          <div className="mb-8">
            <h1 className="text-4xl font-bold text-center mb-2">
              Game Show Button System
            </h1>
            <p className="text-muted-foreground text-center">
              Manage teams and monitor device connections for your game show
            </p>
          </div>

          <div className="grid gap-6 lg:grid-cols-3">
            <div className="lg:col-span-2">
              <TeamsList />
            </div>
            <div>
              <GameStatus />
            </div>
          </div>
        </div>
      </div>
    </WebSocketProvider>
  );
}

export default App;
