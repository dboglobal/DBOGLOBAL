CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1720;
	title = 172002;

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
				conv = 172009;
				ctype = 1;
				idx = 1653110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 172008;
				gtype = 1;
				area = 172001;
				goal = 172004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 172005;
				stype = 2;
				taid = 1;
				title = 172002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 172007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "1653110;";
			}
			CDboTSCheckClrQst
			{
				and = "1705;";
				flink = 1;
				flinknextqid = "1721;1723;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 172008;
			gtype = 1;
			oklnk = 2;
			area = 172001;
			goal = 172004;
			sort = 172005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 172002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 172014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 172001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5411102;";
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

