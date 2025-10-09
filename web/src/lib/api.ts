const API_BASE_URL = import.meta.env.DEV ? "" : "http://localhost:3000";

export interface Team {
  id: string;
  name: string;
  deviceIp?: string;
}

export interface Device {
  ip: string;
}

class ApiClient {
  private async request<T>(
    endpoint: string,
    options: RequestInit = {}
  ): Promise<T> {
    const url = `${API_BASE_URL}${endpoint}`;
    const config: RequestInit = {
      headers: {
        "Content-Type": "application/json",
        ...options.headers,
      },
      ...options,
    };

    const response = await fetch(url, config);

    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    return response.json();
  }

  // Teams API
  async getTeams(): Promise<Team[]> {
    return this.request<Team[]>("/teams");
  }

  async getTeam(id: string): Promise<Team> {
    return this.request<Team>(`/teams/${id}`);
  }

  async createTeam(name: string, deviceIp?: string): Promise<Team> {
    return this.request<Team>("/teams", {
      method: "POST",
      body: JSON.stringify({ name, deviceIp }),
    });
  }

  async updateTeam(id: string, name: string, deviceIp?: string): Promise<Team> {
    return this.request<Team>(`/teams/${id}`, {
      method: "PUT",
      body: JSON.stringify({ name, deviceIp }),
    });
  }

  async deleteTeam(id: string): Promise<void> {
    await this.request<void>(`/teams/${id}`, {
      method: "DELETE",
    });
  }

  // Devices API
  async getDevices(): Promise<string[]> {
    return this.request<string[]>("/devices");
  }
}

export const apiClient = new ApiClient();
