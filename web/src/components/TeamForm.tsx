import { useState } from "react";
import { Button } from "@/components/ui/button";
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogFooter,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Plus, Edit } from "lucide-react";
import type { Team } from "@/lib/api";

interface TeamFormProps {
  onSubmit: (name: string, deviceIp?: string) => Promise<void>;
  team?: Team;
  trigger?: React.ReactNode;
}

export function TeamForm({ onSubmit, team, trigger }: TeamFormProps) {
  const [isOpen, setIsOpen] = useState(false);
  const [name, setName] = useState(team?.name || "");
  const [deviceIp, setDeviceIp] = useState(team?.deviceIp || "");
  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (!name.trim()) return;

    setIsSubmitting(true);
    try {
      await onSubmit(name.trim(), deviceIp.trim() || undefined);
      setIsOpen(false);
      if (!team) {
        setName("");
        setDeviceIp("");
      }
    } catch (error) {
      console.error("Failed to submit form:", error);
    } finally {
      setIsSubmitting(false);
    }
  };

  const defaultTrigger = team ? (
    <Button variant="outline" size="sm">
      <Edit className="h-4 w-4 mr-2" />
      Edit
    </Button>
  ) : (
    <Button size="sm">
      <Plus className="h-4 w-4 mr-2" />
      Add Team
    </Button>
  );

  return (
    <Dialog open={isOpen} onOpenChange={setIsOpen}>
      <DialogTrigger asChild>{trigger || defaultTrigger}</DialogTrigger>
      <DialogContent className="sm:max-w-[425px]">
        <form onSubmit={handleSubmit}>
          <DialogHeader>
            <DialogTitle>{team ? "Edit Team" : "Add New Team"}</DialogTitle>
            <DialogDescription>
              {team
                ? "Update the team information."
                : "Create a new team for the game show."}
            </DialogDescription>
          </DialogHeader>
          <div className="grid gap-4 py-4">
            <div className="grid grid-cols-4 items-center gap-4">
              <Label htmlFor="name" className="text-right">
                Name
              </Label>
              <Input
                id="name"
                value={name}
                onChange={(e) => setName(e.target.value)}
                className="col-span-3"
                placeholder="Team name"
                required
              />
            </div>
            <div className="grid grid-cols-4 items-center gap-4">
              <Label htmlFor="deviceIp" className="text-right">
                Device IP
              </Label>
              <Input
                id="deviceIp"
                value={deviceIp}
                onChange={(e) => setDeviceIp(e.target.value)}
                className="col-span-3"
                placeholder="192.168.1.100 (optional)"
              />
            </div>
          </div>
          <DialogFooter>
            <Button type="submit" disabled={isSubmitting || !name.trim()}>
              {isSubmitting ? "Saving..." : team ? "Update" : "Create"}
            </Button>
          </DialogFooter>
        </form>
      </DialogContent>
    </Dialog>
  );
}
