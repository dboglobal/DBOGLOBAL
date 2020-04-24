CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2649;
	title = 264902;

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
				conv = 264909;
				ctype = 1;
				idx = 4481202;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 264908;
				gtype = 1;
				area = 264901;
				goal = 264904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 264905;
				stype = 1;
				taid = 1;
				title = 264902;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 264908;
			gtype = 1;
			oklnk = 2;
			area = 264901;
			goal = 264904;
			sort = 264905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 264902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 264914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 264901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4192106;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 264907;
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
				npcidx = "4481202;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2650;";
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

