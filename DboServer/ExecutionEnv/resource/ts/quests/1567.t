CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1567;
	title = 156702;

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
				conv = 156709;
				ctype = 1;
				idx = 3142117;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 156708;
				gtype = 1;
				area = 156701;
				goal = 156704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 156705;
				stype = 1;
				taid = 1;
				title = 156702;
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
			stdiag = 156707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "3142117;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1568;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 156708;
			gtype = 1;
			oklnk = 2;
			area = 156701;
			goal = 156704;
			sort = 156705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 156702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 156714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 156701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1651108;";
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

