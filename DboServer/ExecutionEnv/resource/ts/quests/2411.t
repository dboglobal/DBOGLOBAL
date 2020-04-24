CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2411;
	title = 241102;

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
				conv = 241109;
				ctype = 1;
				idx = 3148104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 241108;
				gtype = 1;
				area = 241101;
				goal = 241104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 241105;
				stype = 2;
				taid = 1;
				title = 241102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 241107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "3148104;";
			}
			CDboTSCheckClrQst
			{
				and = "2409;2410;";
				flink = 1;
				flinknextqid = "2412;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 241108;
			gtype = 1;
			oklnk = 2;
			area = 241101;
			goal = 241104;
			sort = 241105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 241102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 241114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 241101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5065103;";
			}
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

