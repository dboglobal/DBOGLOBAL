CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2550;
	title = 255002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 570;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSendSvrEvt
			{
				id = 571;
				tblidx = 2550;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
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
			prelnk = "4;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 255007;
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
				npcidx = "3189105;";
			}
			CDboTSCheckClrQst
			{
				and = "2549;";
				flink = 1;
				flinknextqid = "2551;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 255009;
				ctype = 1;
				idx = 3189105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 255008;
				area = 255001;
				goal = 255004;
				m0fz = "-4473.200195";
				m0widx = 1;
				scitem = -1;
				sort = 255005;
				stype = 2;
				m0fy = "-142.063004";
				m0pat = 0;
				grade = 132203;
				m0fx = "-2396.399902";
				m0ttip = 255015;
				rwd = 100;
				taid = 1;
				title = 255002;
				gtype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 255014;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 255001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3189105;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 255008;
			gtype = 2;
			oklnk = 2;
			area = 255001;
			goal = 255004;
			sort = 255005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 255002;
		}
	}
}

