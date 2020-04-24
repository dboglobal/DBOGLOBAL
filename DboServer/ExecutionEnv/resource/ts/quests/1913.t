CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 15;
	rq = 0;
	tid = 1913;
	title = 191302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSFreeBattle
			{
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 191307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "3183103;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 191309;
				ctype = 1;
				idx = 3183103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 191308;
				gtype = 2;
				area = 191301;
				goal = 191304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 191305;
				stype = 64;
				taid = 1;
				title = 191302;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 191308;
			gtype = 2;
			oklnk = 2;
			area = 191301;
			goal = 191304;
			sort = 191305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 191302;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 191314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 191301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3183103;";
			}
		}
	}
}

