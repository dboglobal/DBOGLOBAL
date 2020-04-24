CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1287;
	title = 128702;

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
				conv = 128709;
				ctype = 1;
				idx = 1551107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 128708;
				gtype = 1;
				area = 128701;
				goal = 128704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 128705;
				stype = 2;
				taid = 1;
				title = 128702;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 128708;
			gtype = 1;
			oklnk = 2;
			area = 128701;
			goal = 128704;
			sort = 128705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 128702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 128714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 128701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142202;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 128707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSClickNPC
			{
				npcidx = "1551107;";
			}
			CDboTSCheckClrQst
			{
				and = "1282;";
				flink = 1;
				not = 0;
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
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

