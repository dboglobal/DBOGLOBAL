CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1245;
	title = 124502;

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
				conv = 124509;
				ctype = 1;
				idx = 5042101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 124508;
				gtype = 1;
				area = 124501;
				goal = 124504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 124505;
				stype = 2;
				taid = 1;
				title = 124502;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 124514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 124501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4111101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 124508;
			gtype = 1;
			oklnk = 2;
			area = 124501;
			goal = 124504;
			sort = 124505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 124502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 124507;
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
				npcidx = "5042101;";
			}
			CDboTSCheckClrQst
			{
				and = "1244;";
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
	}
}

