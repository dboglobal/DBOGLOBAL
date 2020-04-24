CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2429;
	title = 242902;

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
				conv = 242909;
				ctype = 1;
				idx = 6111104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 242908;
				gtype = 1;
				area = 242901;
				goal = 242904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 242905;
				stype = 2;
				taid = 1;
				title = 242902;
			}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 242908;
			gtype = 1;
			oklnk = 2;
			area = 242901;
			goal = 242904;
			sort = 242905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 242902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 242914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 242901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3362102;";
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
			stdiag = 242907;
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
				and = "2421;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "6111104;";
			}
		}
	}
}

