CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2310;
	title = 231002;

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
				conv = 231009;
				ctype = 1;
				idx = 1501104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 231008;
				gtype = 1;
				area = 231001;
				goal = 231004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 231005;
				stype = 1;
				taid = 1;
				title = 231002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 231007;
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
				npcidx = "1501104;";
			}
			CDboTSCheckClrQst
			{
				and = "2309;";
				flink = 1;
				flinknextqid = "2311;";
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
			cont = 231008;
			gtype = 1;
			oklnk = 2;
			area = 231001;
			goal = 231004;
			sort = 231005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 231002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 231014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 231001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242303;";
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

