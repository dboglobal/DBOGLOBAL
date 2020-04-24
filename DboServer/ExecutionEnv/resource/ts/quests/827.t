CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 827;
	title = 82702;

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
				conv = 82709;
				ctype = 1;
				idx = 4071111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 82708;
				gtype = 1;
				area = 82701;
				goal = 82704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 82705;
				stype = 1;
				taid = 1;
				title = 82702;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 82708;
			gtype = 1;
			oklnk = 2;
			area = 82701;
			goal = 82704;
			sort = 82705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 82702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 82714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 82701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3173201;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 82707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 28;
			}
			CDboTSClickNPC
			{
				npcidx = "4071111;";
			}
			CDboTSCheckClrQst
			{
				and = "826;";
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

