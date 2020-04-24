CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 320;
	title = 32002;

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
			desc = 32014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 32001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4152103;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 32008;
			gtype = 2;
			oklnk = 2;
			area = 32001;
			goal = 32004;
			sort = 32005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 32002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 32007;
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
				npcidx = "4152103;";
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

			CDboTSActRegQInfo
			{
				cont = 32008;
				gtype = 2;
				area = 32001;
				goal = 32004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 32005;
				stype = 64;
				taid = 1;
				title = 32002;
			}
			CDboTSActNPCConv
			{
				conv = 32009;
				ctype = 1;
				idx = 4152103;
				taid = 2;
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
				dotype = 0;
			}
		}
	}
}

