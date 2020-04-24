CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2597;
	title = 259702;

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
				conv = 259709;
				ctype = 1;
				idx = 8531101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 259708;
				gtype = 1;
				area = 259701;
				goal = 259704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 259705;
				stype = 1;
				taid = 1;
				title = 259702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 259714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 259701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6312111;";
			}
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
			cont = 259708;
			gtype = 1;
			oklnk = 2;
			area = 259701;
			goal = 259704;
			sort = 259705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 259702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 259707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "8531101;";
			}
			CDboTSCheckClrQst
			{
				and = "2596;";
				flink = 1;
				flinknextqid = "2598;";
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
	}
}

