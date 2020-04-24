CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 650;
	title = 65002;

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
				conv = 65009;
				ctype = 1;
				idx = 1351202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 65008;
				gtype = 1;
				area = 65001;
				goal = 65004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 65005;
				stype = 2;
				taid = 1;
				title = 65002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 65007;
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
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "1351202;";
			}
			CDboTSCheckClrQst
			{
				and = "649;";
				flink = 1;
				flinknextqid = "674;652;";
				not = 0;
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
			cont = 65008;
			gtype = 1;
			oklnk = 2;
			area = 65001;
			goal = 65004;
			sort = 65005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 65002;
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
			desc = 65014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 65001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3182102;";
			}
		}
	}
}

