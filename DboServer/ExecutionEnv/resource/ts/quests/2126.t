CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2126;
	title = 212602;

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
				conv = 212609;
				ctype = 1;
				idx = 2801113;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 212608;
				gtype = 1;
				area = 212601;
				goal = 212604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 212605;
				stype = 2;
				taid = 1;
				title = 212602;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 212608;
			gtype = 1;
			oklnk = 2;
			area = 212601;
			goal = 212604;
			sort = 212605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 212602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 212614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 212601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1471107;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 212607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "2801113;";
			}
			CDboTSCheckClrQst
			{
				and = "2125;";
				flink = 1;
				flinknextqid = "2127;";
				not = 0;
			}
		}
	}
}

