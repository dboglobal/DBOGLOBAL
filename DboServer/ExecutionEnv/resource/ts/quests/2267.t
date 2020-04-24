CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2267;
	title = 226702;

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
				conv = 226709;
				ctype = 1;
				idx = 6412112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 226708;
				gtype = 1;
				area = 226701;
				goal = 226704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 226705;
				stype = 1;
				taid = 1;
				title = 226702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 226707;
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
				npcidx = "6412112;";
			}
			CDboTSCheckClrQst
			{
				and = "2262;";
				flink = 0;
				flinknextqid = "2268;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 226708;
			gtype = 1;
			oklnk = 2;
			area = 226701;
			goal = 226704;
			sort = 226705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 226702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 226714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 226701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1551112;";
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

