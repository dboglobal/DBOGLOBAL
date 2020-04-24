CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2471;
	title = 247102;

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
				conv = 247109;
				ctype = 1;
				idx = 1655203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 247108;
				gtype = 1;
				area = 247101;
				goal = 247104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 247105;
				stype = 2;
				taid = 1;
				title = 247102;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 247114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 247101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653115;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 247108;
			gtype = 1;
			oklnk = 2;
			area = 247101;
			goal = 247104;
			sort = 247105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 247102;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 247107;
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
				and = "2469;";
				flink = 1;
				flinknextqid = "2474;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1655203;";
			}
		}
	}
}

