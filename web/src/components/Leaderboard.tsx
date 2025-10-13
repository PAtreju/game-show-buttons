import { useState, useEffect } from "react";
import {
  Card,
  CardContent,
  CardDescription,
  CardHeader,
  CardTitle,
} from "@/components/ui/card";
import {
  Table,
  TableBody,
  TableCell,
  TableHead,
  TableHeader,
  TableRow,
} from "@/components/ui/table";
import { Badge } from "@/components/ui/badge";
import { Trophy, Zap } from "lucide-react";
import { apiClient, type Team } from "@/lib/api";
import { useWebSocket } from "@/hooks/useWebSocketHook";

export function Leaderboard() {
  const [teams, setTeams] = useState<Team[]>([]);
  const [loading, setLoading] = useState(true);
  const [pressedDevice, setPressedDevice] = useState<string | null>(null);
  const { lastMessage } = useWebSocket();

  const fetchTeams = async () => {
    try {
      setLoading(true);
      const data = await apiClient.getTeams();
      // Sort teams by points descending
      const sortedTeams = data.sort((a, b) => (b.points || 0) - (a.points || 0));
      setTeams(sortedTeams);
    } catch (err) {
      console.error("Failed to fetch teams:", err);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchTeams();
  }, []);

  // Handle WebSocket messages for button press
  useEffect(() => {
    if (lastMessage) {
      switch (lastMessage.type) {
        case "buttonPressed":
          setPressedDevice(lastMessage.ip || null);
          fetchTeams(); // Refresh to show updated points
          break;
        case "reset":
          setPressedDevice(null);
          fetchTeams(); // Refresh to show updated points
          break;
      }
    }
  }, [lastMessage]);

  if (loading) {
    return (
      <div className="flex items-center justify-center min-h-screen">
        <p className="text-muted-foreground">Loading leaderboard...</p>
      </div>
    );
  }

  return (
    <div className="min-h-screen bg-background">
      <div className="container mx-auto py-8 px-4">
        <div className="mb-8 text-center">
        </div>

        {teams.length === 0 ? (
          <Card>
            <CardContent className="py-8">
              <p className="text-center text-muted-foreground">
                No teams created yet.
              </p>
            </CardContent>
          </Card>
        ) : (
          <div className="flex min-w-full gap-6">
            {teams.map((team, index) => (
              <Card key={team.id} className="relative overflow-hidden w-full">
                {index === 0 && (
                  <div className="absolute top-0 left-0 w-full h-2 bg-gradient-to-r from-yellow-400 via-yellow-500 to-yellow-600" />
                )}
                {index === 1 && (
                  <div className="absolute top-0 left-0 w-full h-2 bg-gradient-to-r from-gray-300 via-gray-400 to-gray-500" />
                )}
                {index === 2 && (
                  <div className="absolute top-0 left-0 w-full h-2 bg-gradient-to-r from-orange-400 via-orange-500 to-orange-600" />
                )}
                <CardHeader>
                  <div className="flex items-center justify-between">
                    <div className="flex items-center gap-4">
                      <div className="text-4xl font-bold text-muted-foreground">
                        #{index + 1}
                      </div>
                      <div>
                        <CardTitle className="text-3xl">{team.name}</CardTitle>
                        <CardDescription>
                          {team.players?.length || 0} player{team.players?.length !== 1 ? 's' : ''}
                        </CardDescription>
                      </div>
                    </div>
                    <div className="text-right">
                      <div className="text-5xl font-bold text-primary">
                        {team.points || 0}
                      </div>
                      <div className="text-sm text-muted-foreground">points</div>
                    </div>
                  </div>
                </CardHeader>
                <CardContent>
                  {team.players && team.players.length > 0 ? (
                    <Table>
                      <TableHeader>
                        <TableRow>
                          <TableHead>Player Name</TableHead>
                          <TableHead>Status</TableHead>
                        </TableRow>
                      </TableHeader>
                      <TableBody>
                        {team.players.map((player) => {
                          const isPressed = !!player.deviceIp && !!pressedDevice && player.deviceIp === pressedDevice;
                          return (
                            <TableRow 
                              key={player.id}
                              className={isPressed ? "bg-yellow-100" : ""}
                            >
                              <TableCell className="font-medium">
                                <div className="flex items-center gap-2">
                                  {player.name}
                                  {isPressed && (
                                    <Badge variant="destructive" className="animate-pulse">
                                      <Zap className="h-3 w-3 mr-1" />
                                      PRESSED!
                                    </Badge>
                                  )}
                                </div>
                              </TableCell>
                              <TableCell>
                                {player.deviceIp ? (
                                  <Badge variant="default">Connected</Badge>
                                ) : (
                                  <Badge variant="secondary">Not Assigned</Badge>
                                )}
                              </TableCell>
                            </TableRow>
                          );
                        })}
                      </TableBody>
                    </Table>
                  ) : (
                    <p className="text-center text-muted-foreground py-4">
                      No players in this team
                    </p>
                  )}
                </CardContent>
              </Card>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}
