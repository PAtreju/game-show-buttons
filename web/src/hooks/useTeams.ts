import { useState, useEffect } from "react";
import { apiClient, type Team } from "../lib/api";

export function useTeams() {
  const [teams, setTeams] = useState<Team[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  const fetchTeams = async () => {
    try {
      setLoading(true);
      setError(null);
      const data = await apiClient.getTeams();
      setTeams(data);
    } catch (err) {
      setError(err instanceof Error ? err.message : "Failed to fetch teams");
    } finally {
      setLoading(false);
    }
  };

  const createTeam = async (name: string, deviceIp?: string) => {
    try {
      setError(null);
      const newTeam = await apiClient.createTeam(name, deviceIp);
      setTeams((prev) => [...prev, newTeam]);
      return newTeam;
    } catch (err) {
      setError(err instanceof Error ? err.message : "Failed to create team");
      throw err;
    }
  };

  const updateTeam = async (id: string, name: string, deviceIp?: string) => {
    try {
      setError(null);
      const updatedTeam = await apiClient.updateTeam(id, name, deviceIp);
      setTeams((prev) =>
        prev.map((team) => (team.id === id ? updatedTeam : team))
      );
      return updatedTeam;
    } catch (err) {
      setError(err instanceof Error ? err.message : "Failed to update team");
      throw err;
    }
  };

  const deleteTeam = async (id: string) => {
    try {
      setError(null);
      await apiClient.deleteTeam(id);
      setTeams((prev) => prev.filter((team) => team.id !== id));
    } catch (err) {
      setError(err instanceof Error ? err.message : "Failed to delete team");
      throw err;
    }
  };

  useEffect(() => {
    fetchTeams();
  }, []);

  return {
    teams,
    loading,
    error,
    refetch: fetchTeams,
    createTeam,
    updateTeam,
    deleteTeam,
  };
}
