CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 285;
	title = 28502;

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
				conv = 28509;
				ctype = 1;
				idx = 4751106;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 28508;
				gtype = 1;
				area = 28501;
				goal = 28504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 28505;
				stype = 2;
				taid = 1;
				title = 28502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 28508;
			gtype = 1;
			oklnk = 2;
			area = 28501;
			goal = 28504;
			sort = 28505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 28502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 28514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 28501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 28507;
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
				minlvl = 12;
			}
			CDboTSClickNPC
			{
				npcidx = "4751106;";
			}
			CDboTSCheckClrQst
			{
				and = "283;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
	}
}

