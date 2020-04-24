CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 720;
	title = 72002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 72009;
				ctype = 1;
				idx = 3173101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 72008;
				gtype = 1;
				area = 72001;
				goal = 72004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 72005;
				stype = 2;
				taid = 1;
				title = 72002;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 72008;
			gtype = 1;
			oklnk = 2;
			area = 72001;
			goal = 72004;
			sort = 72005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 72002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 72014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 72001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4152112;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 72007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 24;
			}
			CDboTSClickNPC
			{
				npcidx = "3173101;";
			}
			CDboTSCheckClrQst
			{
				and = "693;";
				flink = 1;
				flinknextqid = "721;";
				not = 0;
			}
		}
	}
}

