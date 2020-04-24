CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 318;
	title = 31802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActOutMsg
			{
				idx = 115;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 31814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 31801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8032126;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 31808;
			gtype = 1;
			oklnk = 2;
			area = 31801;
			goal = 31804;
			sort = 31805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 31802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 31807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 5;
			}
			CDboTSClickNPC
			{
				npcidx = "8032126;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
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
				conv = 31809;
				ctype = 1;
				idx = 8032126;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 31808;
				gtype = 1;
				area = 31801;
				goal = 31804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 31805;
				stype = 64;
				taid = 1;
				title = 31802;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSDialogOpen
			{
				dotype = 2;
			}
		}
	}
}

