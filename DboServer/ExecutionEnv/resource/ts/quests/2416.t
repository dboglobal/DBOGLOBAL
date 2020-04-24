CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2416;
	title = 241602;

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
				conv = 241609;
				ctype = 1;
				idx = 4072302;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 241608;
				gtype = 1;
				area = 241601;
				goal = 241604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 241605;
				stype = 2;
				taid = 1;
				title = 241602;
			}
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
			desc = 241614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 241601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072301;";
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
			cont = 241608;
			gtype = 1;
			oklnk = 2;
			area = 241601;
			goal = 241604;
			sort = 241605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 241602;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 241607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSCheckClrQst
			{
				and = "2415;";
				flink = 1;
				flinknextqid = "2417;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4072302;";
			}
		}
	}
}

