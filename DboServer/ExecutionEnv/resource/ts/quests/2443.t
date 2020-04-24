CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2443;
	title = 244302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWorldPlayScript
			{
				sid = 2443;
				start = 0;
				taid = 1;
				uof = 0;
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
			prelnk = "101;";
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
			stdiag = 244307;
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
				and = "2442;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2802103;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 552;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 244314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 244301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2802103;";
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
				conv = 244309;
				ctype = 1;
				idx = 2802103;
				taid = 2;
			}
			CDboTSActWorldPlayScript
			{
				sid = 2443;
				start = 1;
				taid = 3;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 244308;
				gtype = 3;
				area = 244301;
				goal = 244304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 244305;
				stype = 2;
				taid = 1;
				title = 244302;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 244308;
			gtype = 3;
			oklnk = 2;
			area = 244301;
			goal = 244304;
			sort = 244305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 244302;
		}
	}
}

