CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 220;
	title = 22002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 22008;
				gtype = 1;
				area = 22001;
				goal = 22004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 22005;
				stype = 1;
				taid = 1;
				title = 22002;
			}
			CDboTSActNPCConv
			{
				conv = 22009;
				ctype = 1;
				idx = 4751301;
				taid = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			desc = 22014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 22001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511601;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 22007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "4751301;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "222;";
				not = 0;
				or = "219;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 22008;
			gtype = 1;
			oklnk = 2;
			area = 22001;
			goal = 22004;
			sort = 22005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 22002;
		}
	}
}

