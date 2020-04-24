CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1735;
	title = 173502;

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
				conv = 173509;
				ctype = 1;
				idx = 6313104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 173508;
				gtype = 1;
				area = 173501;
				goal = 173504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 173505;
				stype = 1;
				taid = 1;
				title = 173502;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 173507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "6313104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1736;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 173508;
			gtype = 1;
			oklnk = 2;
			area = 173501;
			goal = 173504;
			sort = 173505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 173502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 173514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 173501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5063105;";
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

